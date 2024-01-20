#pragma once

namespace resource {

    struct texture {
        unsigned int id;
        unsigned int width, height, channels;

        static texture load(const char* filepath);
    };

} // namespace resource
