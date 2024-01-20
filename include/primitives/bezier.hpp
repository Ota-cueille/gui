#pragma once

#include <glm/vec2.hpp>

namespace primitive {

    struct bezier {
        unsigned int id;
        glm::vec2 start, end;
        glm::vec2 control1, control2;

        struct parameters {
            glm::vec2 start, end;
            glm::vec2 control1, control2;
        };
    };

} // namespace renderer::primitive
