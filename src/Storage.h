#pragma once
#include "StoreBase.h"

namespace Store {

	template<typename Base>
	class Storage {
	public:
		using PtrDecodeFunc = std::function<void(void*, std::ifstream&)>;
		using PtrEncodeFunc = std::function<void(void*, std::ofstream&)>;
		using SetFunc = std::function<void(Base*, std::ifstream&)>;
		using GetFunc = std::function<void(Base*, std::ofstream&)>;
		using Offset = unsigned int;

		static void checkRepeated(const std::string& name) {
			if (getInstance()->setterFunc.find(name) != getInstance()->setterFunc.end() ||
				getInstance()->ptrDecodeFunc.find(name) != getInstance()->ptrDecodeFunc.end()) {
				std::cout << "Error : " << name << " is Repeated\n";
				exit(-1);
			}
		}

		template<typename T, typename V = void>
		struct Public {
			Public() = delete;
		};

		/*
		int
		string
		*/
		template<typename T>
		struct Public<T, std::enable_if_t<has_fstream_v<T> && !std::is_base_of_v<Storable, get_origin_t<T>>>> {
			Public(const std::string& name, Offset off) {
				Storage<Base>::checkRepeated(name);
				auto decoder = [](void* ptr, std::ifstream& stream) {stream >> (*((T*)ptr)); };
				auto encoder = [](void* ptr, std::ofstream& stream) {stream << (*((T*)ptr)) << ' '; };
				getInstance()->ptrDecodeFunc[name] = decoder;
				getInstance()->ptrEncodeFunc[name] = encoder;
				getInstance()->offset[name] = off;
			}
		};

		/*
		vector<int>
		*/
		template<typename T>
		struct Public<T, std::enable_if_t<
			has_push_back_v<T>&& 
			has_fstream_v<typename T::value_type>&&
			is_container_v<T>
			>> {
			Public(const std::string& name, Offset off) {
				Storage<Base>::checkRepeated(name);
				using inner_value_type = typename T::value_type;
				auto decoder = [](void* ptr, std::ifstream& stream) {
					int size; stream >> size;
					T& container = *reinterpret_cast<T*>(ptr);
					for (int i = 0; i < size; i++) {
						inner_value_type tmp;
						stream >> tmp;
						container.push_back(tmp);
					}
				};
				auto encoder = [](void* ptr, std::ofstream& stream) {
					T& container = *reinterpret_cast<T*>(ptr);
					stream << container.size() << ' ';
					for (inner_value_type& it : container) {
						stream << it << ' ';
					}
				};
				getInstance()->ptrDecodeFunc[name] = decoder;
				getInstance()->ptrEncodeFunc[name] = encoder;
				getInstance()->offset[name] = off;
			}
		};

		/*
		int*
		*/
		template<typename T>
		struct Public<T, std::enable_if_t<std::is_pointer_v<T> && !std::is_base_of_v<Storable, get_origin_t<T>>>> {
			Public(const std::string& name, Offset off) {
				Storage<Base>::checkRepeated(name);
				auto decoder = [](void* ptr, std::ifstream& stream) {
					*static_cast<T*>(ptr) = new std::remove_pointer_t<T>();
					stream >> (**static_cast<T*>(ptr));
				};
				auto encoder = [](void* ptr, std::ofstream& stream) { stream << (**static_cast<T*>(ptr)) << ' '; };
				getInstance()->ptrDecodeFunc[name] = decoder;
				getInstance()->ptrEncodeFunc[name] = encoder;
				getInstance()->offset[name] = off;
			}
		};

		/*
		A* (storeable)
		*/
		template<typename T>
		struct Public<T, std::enable_if_t<
			std::is_base_of_v<Storable, get_origin_t<T>> && 
			std::is_pointer_v<T>
			>> {
			Public(const std::string& name, Offset off) {
				Storage<Base>::checkRepeated(name);
				auto decoder = [=](void* ptr, std::ifstream& stream) {
					StoreID id; stream >> id;
					Storable* memory = Store::getStorable(id);
					if (memory != nullptr)
						*static_cast<Storable**>(ptr) = memory;
					else {
						*static_cast<StoreID*>(ptr) = id;
						addDependency(static_cast<Storable*>(ptr));
					}
				};
				auto encoder = [](void* ptr, std::ofstream& stream) {
					stream << (**static_cast<Storable**>(ptr)).getStoreID() << ' ';
					addDependency(*static_cast<Storable**>(ptr));
				};
				getInstance()->ptrDecodeFunc[name] = decoder;
				getInstance()->ptrEncodeFunc[name] = encoder;
				getInstance()->offset[name] = off;
			}
		};
		
		/*
		vector<A*(public Storable)>
		*/
		template<typename T>
		struct Public<T, std::enable_if_t<
			has_push_back_v<T>&&
			std::is_base_of_v<Storable, get_origin_t<typename T::value_type>>&&
			std::is_pointer_v<typename T::value_type>
			>> {
			Public(const std::string& name, Offset off) {
				using inner_value_type = typename T::value_type;
				Storage<Base>::checkRepeated(name);
				auto decoder = [=](void* ptr, std::ifstream& stream) {
					int size; stream >> size;
					T& container = *reinterpret_cast<T*>(ptr);
					container.clear();
					std::vector<StoreID> ids;
					for (int i = 0; i < size; i++) {
						StoreID id; stream >> id;
						ids.push_back(id);
						container.push_back(nullptr);
					}
					for (int i = 0; i < size; i++) {
						container[i] = (inner_value_type)ids[i];
						addDependency((Storable*)&container[i]);
					}
				};
				auto encoder = [](void* ptr, std::ofstream& stream) {
					T& container = *reinterpret_cast<T*>(ptr);
					stream << container.size() << ' ';
					for (Storable* it : container) {
						stream << it->getStoreID() << ' ';
						addDependency(it);
					}
				};
				getInstance()->ptrDecodeFunc[name] = decoder;
				getInstance()->ptrEncodeFunc[name] = encoder;
				getInstance()->offset[name] = off;
			}
		};

		template<typename T, typename V = void>
		struct Private {
			Private() = delete;
		};

		template<typename T>
		struct Private<T, std::enable_if_t<has_fstream_in_v<T> && !std::is_base_of_v<Storable, get_origin_t<T>>>> {
			Private(const std::string& name, T(Base::* getter)(), void(Base::* setter)(T)) {
				Storage<Base>::checkRepeated(name);
				auto decoder = [=](Base* base, std::ifstream& stream) {T value; stream >> value; (base->*setter)(value); };
				auto encoder = [=](Base* base, std::ofstream& stream) {stream << (base->*getter)() << ' '; };
				getInstance()->setterFunc[name] = decoder;
				getInstance()->getterFunc[name] = encoder;
			}
		};
		
		/*
		A*(Storable)
		*/
		template<typename T>
		struct Private<T, std::enable_if_t<
			std::is_base_of_v<Storable, get_origin_t<T>>
			>> {
			Private(const std::string& name, T(Base::* getter)(), void(Base::* setter)(T)) {
				Storage<Base>::checkRepeated(name);
				auto decoder = [=](Base* base, std::ifstream& stream) {
					StoreID id; stream >> id;
					Storable* memory = Store::getStorable(id);
					if (memory != nullptr)
						(base->*setter)(static_cast<T>(memory));
					else {
						(base->*setter)((T)(id));
						std::function<Storable* (void)> get = [=]() {return ((Storable*)(base->*getter)()); };
						std::function<void(Storable*)> set = [=](Storable* item) { (base->*setter)((T)(item)); };
						addDependency(get, set);
					}
				};
				auto encoder = [=](Base* base, std::ofstream& stream) {
					stream << (base->*getter)()->getStoreID() << ' ';
					addDependency((base->*getter)());
				};
				getInstance()->setterFunc[name] = decoder;
				getInstance()->getterFunc[name] = encoder;
			}
			Private(const std::string& name, T(Base::* getter)() const, void(Base::* setter)(T)) {
				Storage<Base>::checkRepeated(name);
				auto decoder = [=](Base* base, std::ifstream& stream) {
					StoreID id; stream >> id;
					Storable* memory = Store::getStorable(id);
					if (memory != nullptr)
						(base->*setter)(static_cast<T>(memory));
					else {
						(base->*setter)((T)(id));
						std::function<Storable* (void)> get = [=]() {return ((Storable*)(base->*getter)()); };
						std::function<void(Storable*)> set = [=](Storable* item) { (base->*setter)((T)(item)); };
						addDependency(get, set);
					}
				};
				auto encoder = [=](Base* base, std::ofstream& stream) {
					stream << (base->*getter)()->getStoreID() << ' ';
					addDependency((base->*getter)());
				};
				getInstance()->setterFunc[name] = decoder;
				getInstance()->getterFunc[name] = encoder;
			}
		};

		bool decode(Base* base, const std::string& name, std::ifstream& stream) {
			std::cout << "decode name=" << name << "\n";
			if (decodeFromPtr(base, name, stream))return true;
			if (decodeFromSetter(base, name, stream))return true;
			return false;
		}
		void encode(Base* base, std::ofstream& stream) {
			for (auto& it : ptrEncodeFunc) {
				void* ptr = (void*)((char*)base + offset[it.first]);
				stream << it.first << ' ';
				(it.second)(ptr, stream);
			}
			for (auto& it : getterFunc) {
				stream << it.first << ' ';
				(it.second)(base, stream);
			}
		}
		static Storage* getInstance() {
			static Storage f;
			return &f;
		}
	private:
		bool decodeFromPtr(void* base, const std::string& name, std::ifstream& stream) {
			if (ptrDecodeFunc.find(name) ==
				ptrDecodeFunc.end())return false;
			void* ptr = (void*)((char*)base + (offset[name]));
			ptrDecodeFunc[name](ptr, stream);
			return true;
		}
		bool decodeFromSetter(Base* base, const std::string& name, std::ifstream& stream) {
			if (setterFunc.find(name) ==
				setterFunc.end())return false;
			setterFunc[name](base, stream);
			return true;
		}
		Storage() {};
		Storage(const Storage&) = delete;
		Storage(Storage&&) = delete;
		Storage& operator=(const Storage&) = delete;
		std::map<std::string, PtrDecodeFunc> ptrDecodeFunc;
		std::map<std::string, PtrEncodeFunc> ptrEncodeFunc;
		std::map<std::string, SetFunc> setterFunc;
		std::map<std::string, GetFunc> getterFunc;
		std::map<std::string, Offset> offset;
	};
}