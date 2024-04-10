#include "ToText.h"

#include <iostream>
#include <cassert>

using namespace std;

enum DAY_OF_WEEK {
    Mo, Tu, We, Th, Fr, Sa, Su
};

enum class STATUS {
    Happy, Moody, Angry
};

class Record: public ttx::Storable {
    TTX_DECLARE(Record);
public:
    Record(DAY_OF_WEEK day, STATUS mood): day(day), mood(mood) {}
    DAY_OF_WEEK day;
    STATUS mood;
};
TTX_IMPL(Record);
TTX_VARIABLE_PUB(Record, day);
TTX_VARIABLE_PUB(Record, mood);

Record* record(DAY_OF_WEEK day, STATUS mood) {
    return new Record(day, mood);
}

int main() {
    vector<ttx::Storable*> testdata = {
        record(Mo, STATUS::Moody),
        record(Tu, STATUS::Happy),
        record(We, STATUS::Angry),
        record(Th, STATUS::Happy),
        record(Fr, STATUS::Happy),
        record(Sa, STATUS::Happy),
        record(Su, STATUS::Moody)
    };

    ttx::store(cout, testdata);

    ttx::store("./temp.txt", testdata);

    vector<ttx::Storable*> result = ttx::load("./temp.txt");
    assert(result.size() == testdata.size());
    for (int i = 0; i < result.size(); i++) {
        Record* a = dynamic_cast<Record*>(result[i]);
        Record* b = dynamic_cast<Record*>(testdata[i]);
        assert(a->day == b->day);
        assert(a->mood == b->mood);
    }

    cout << "test enum passed\n";
    return 0;
}