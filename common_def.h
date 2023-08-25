#ifndef COMMON_DEF_H
#define COMMON_DEF_H


#define DISABLE_COPY(Type) \
    Type(const Type &); \
    Type& operator=(const Type &); \
    Type(Type &&); \
    Type& operator=(Type &&);

#define DELETE_OBJECT(obj) \
    if (obj != nullptr) { \
        delete obj; \
        obj = nullptr; \
    }

#define DELETE_ARRAY(array) \
    if (array != nullptr) { \
        delete [] array; \
        array = nullptr; \
    }

#define FREE_OBJECT(obj) \
    if (obj != nullptr) { \
        std::free(obj); \
        obj = nullptr; \
    }

#define FREE_ARRAY(array) \
    if (array != nullptr) { \
        std::free(array); \
        array = nullptr; \
    }


#define DECLARE_INSTANCE(Type) \
    inline static Type& instance() { \
        static Type typeInstance; \
        return typeInstance; \
    }


#endif // COMMON_DEF_H
