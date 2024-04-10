#pragma once

#include "Storable.h"
#include "Traiter.h"
#include "Store.h"

namespace ttx {

void addDependency(Storable* item);
void file2string(void* ptr);
std::string string2file(void* str);

/* Storage���ṩ�˶Բ�ͬ�������͵�encode/decode����
*  ÿһ���̳���Storable���඼�����Լ���Storage��
*  Storage�б�����ÿ��Storable�� <�����л�������>
*/
template<typename Base>
class Storage {
public:
    using DecodeFunc = std::function<void(void*, std::istream&)>;
    using EncodeFunc = std::function<void(void*, std::ostream&)>;
    using Offset = unsigned;
    struct Accessor {
        DecodeFunc decode;
        EncodeFunc encode;
        Offset offset;
    };

    static Storage* getInstance() {
        static Storage f;
        return &f;
    }

    bool decode(Base* base, const std::string& name, std::istream& stream) {
        if (nameToAccessor.find(name) ==
            nameToAccessor.end())return false;
        char* pb = static_cast<char*>(static_cast<void*>(base));
        const Accessor& accessor = nameToAccessor[name];
        void* ptr = static_cast<void*>(pb + accessor.offset);
        accessor.decode(ptr, stream);
        return true;
    }

    void encode(Base* base, std::ostream& stream) {
        char* pb = static_cast<char*>(static_cast<void*>(base));
        for (auto& it : nameToAccessor) {
            Accessor& accessor = it.second;
            void* ptr = static_cast<void*>(pb + accessor.offset);
            stream << it.first << ' ';
            accessor.encode(ptr, stream);
        }
    }

    void insert(const std::string& name,
                const DecodeFunc& decoder,
                const EncodeFunc& encoder,
                Offset offset) {
        if (nameToAccessor.find(name) != nameToAccessor.end()) {
            std::cout << "Error : " << name << " is Repeated\n";
            exit(-233);
        }
        nameToAccessor[name] = { decoder, encoder, offset };
    }
private:
    Storage() {};
    Storage(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage& operator=(Storage&&) = delete;
    std::map<std::string, Accessor> nameToAccessor;


public:
    template<typename T, typename V = void>
    struct Variable {
        Variable() = delete;
        Variable(const std::string& name,   // ��ĳ�Ա������
                 Offset offset)             // ��Ա������������ƫ����
        = delete;
    };

    // ����֧�� << �� >> ��������Ļ�������
    template<typename T>
    struct Variable<T,
        std::enable_if_t<
            has_stream_v<T> && 
            !std::is_pointer_v<T> &&
            !std::is_same_v<T, std::string>
            >
        > {
        Variable(const std::string& name, Offset offset) {
            getInstance()->insert(name, getDecoder(), getEncoder(), offset);
        }
        static DecodeFunc getDecoder() {
            return [](void* ptr, std::istream& stream) {
                T* location = static_cast<T*>(ptr);
                stream >> *location;
            };
        }
        static EncodeFunc getEncoder() {
            return [](void* ptr, std::ostream& stream) {
                T* location = static_cast<T*>(ptr);
                T& value = *location;
                stream << value << ' ';
            };
        }
    };

    // ֧�� string ����
    template<typename T>
    struct Variable<T, 
        std::enable_if_t<
            std::is_same_v<T, std::string>
        >
    > {
        Variable(const std::string name, Offset offset) {
            getInstance()->insert(name, getDecoder(), getEncoder(), offset);
        }
        static DecodeFunc getDecoder() {
            return [](void* ptr, std::istream& stream) {
                T* location = static_cast<T*>(ptr);
                stream >> *location;
                file2string(ptr);
            };
        }
        static EncodeFunc getEncoder() {
            return [](void* ptr, std::ostream& stream) {
                T* location = static_cast<T*>(ptr);
                T& value = *location;
                stream << string2file(ptr) << ' ';
            };
        }
    };

    // ֧��ö������
    template<typename T>
    struct Variable<T, 
        std::enable_if_t<
            std::is_enum_v<T>
        >
    > {
        using B = std::underlying_type_t<T>;
        Variable(const std::string& name, Offset offset) {
            getInstance()->insert(name, getDecoder(), getEncoder(), offset);
        }
        static DecodeFunc getDecoder() {
            return [](void* ptr, std::istream& stream) {
                auto decoder = Variable<B>::getDecoder();
                decoder(ptr, stream);
            };
        }
        static EncodeFunc getEncoder() {
            return [](void* ptr, std::ostream& stream) {
                auto encoder = Variable<B>::getEncoder();
                encoder(ptr, stream);
            };
        }
    };

    // ֧���κο��Ե����ģ�ӵ��push_back��������ͨԪ������
    template<typename T>
    struct Variable<T,
        std::enable_if_t<
            has_push_back_v<T> &&
            is_container_v<T>
        >
    > {
        using I = typename T::value_type;
        Variable(const std::string& name, Offset offset) {
            getInstance()->insert(name, getDecoder(), getEncoder(), offset);
        }
        static DecodeFunc getDecoder() {
            return [](void* ptr, std::istream& stream) {
                int size;
                auto decoder = Variable<I>::getDecoder();
                T* location = static_cast<T*>(ptr);
                T& container = *location;
                stream >> size;
                container.resize(size);
                for (auto& element: container) {
                    decoder(static_cast<void*>(&element), stream);
                }
            };
        }
        static EncodeFunc getEncoder() {
            return [](void* ptr, std::ostream& stream) {
                T* location = static_cast<T*>(ptr);
                T& container = *location;
                auto encoder = Variable<I>::getEncoder();
                stream << container.size() << ' ';
                for (auto& element: container) {
                    encoder(static_cast<void*>(&element), stream);
                }
            };
        }
    };

    // ��Storable��ָ�����ͣ��ؽ�ʱ���ᱣ�����ü����ϵ
    template<typename T> // T = BasicType*
    struct Variable<T,
        std::enable_if_t<
            std::is_pointer_v<T> &&
            !std::is_base_of_v<Storable, get_origin_t<T>>
        >
    > {
        using B = std::remove_pointer_t<T>;
        Variable(const std::string& name, Offset offset) {
            getInstance()->insert(name, getDecoder(), getEncoder(), offset);
        }
        static DecodeFunc getDecoder() {
            return [](void* ptr, std::istream& stream) {
                T* location = static_cast<T*>(ptr);
                *location = new B();
                auto decoder = Variable<B>::getDecoder();
                decoder(static_cast<void*>(*location), stream);
            };
        }
        static EncodeFunc getEncoder() {
            return [](void* ptr, std::ostream& stream) {
                auto encoder = Variable<B>::getEncoder();
                T* location = static_cast<T*>(ptr);
                encoder(static_cast<void*>(*location), stream);
            };
        }
    };

    // Storable��ָ�����ͣ��ؽ�ʱ�ᱣ�����ü����ϵ
    template<typename T>    // T = Storable*
    struct Variable<T,
        std::enable_if_t<
            std::is_pointer_v<T> &&
            std::is_base_of_v<Storable, get_origin_t<T>>
        >    
    > {
        using B = std::remove_pointer_t<T>; // B = Storable
        Variable(const std::string& name, Offset offset) {
            getInstance()->insert(name, getDecoder(), getEncoder(), offset);
        }
        static DecodeFunc getDecoder() {
            return [](void* ptr, std::istream& stream) {
                StoreID id;
                T* location = static_cast<T*>(ptr);
                stream >> id;
                if (id == 0)  *location = nullptr;
                else {
                    *static_cast<StoreID*>(ptr) = id;
                    addDependency(static_cast<B*>(ptr));
                }
            };
        }
        static EncodeFunc getEncoder() {
            return [](void* ptr, std::ostream& stream) {
                if (*static_cast<T*>(ptr) == nullptr) stream << 0 << ' ';
                else {
                    stream << (**static_cast<T*>(ptr)).getStoreID() << ' ';
                    addDependency(*static_cast<T*>(ptr));
                }
            };
        }
    };
};

}