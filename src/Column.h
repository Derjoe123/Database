#include <stdbool.h>
#include <stdio.h>
enum ColumnType { INT, FLOAT, STRING, DATE, BOOL, BIGINT, BIGFLOAT };
typedef struct {
    char Day;
    char Month;
    long Year;
} Date;
typedef struct {
    char *name;
    enum ColumnType type;
    void *data;
    size_t dataSize;
    bool fixedSizeType;
} Column;

size_t getTypeSize(enum ColumnType type);
