#pragma once

namespace love
{
    class Message : public Object
    {
        public:
            Message(const std::string & name, const std::vector<Variant> & args = {});
            ~Message() {};

            int ToLua(lua_State * L);

        private:
            const std::string name;
            const std::vector<Variant> args;
    };
}
