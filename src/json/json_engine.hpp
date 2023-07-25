#ifndef _JSON_ENGINE_HPP
#define _JSON_ENGINE_HPP


namespace qbackend {
    class json_engine {
    public:
        json_engine();

        ~json_engine();


        char *encode(const void *obj, char *(*callback)(const void *obj));
        void *decode(const char *json, void *(*callback)(const char *json));
    
        void *json_load(const char *filename, void *(*callback)(const char *json));
        int json_save(const void *obj, const char *filename, char *(*callback)(const void *obj));

    };
};

#endif // JSON_ENGINE_HPP