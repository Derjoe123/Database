#include "Column.h"

size_t getTypeSize(enum ColumnType type) {
    switch (type) {
    case INT:
        return sizeof(int);
    case FLOAT:
        return sizeof(float);
    case STRING:
        return 0;
    case BOOL:
        return sizeof(bool);
    case BIGINT:
        return sizeof(long long);
    case BIGFLOAT:
        return sizeof(double);
    case DATE:
        return sizeof(Date);
    }
    return 0;
}
