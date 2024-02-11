struct Base {
    int Line;

    constexpr Base(int line = __builtin_LINE())
        : Line(line) {} 
};

struct Test
    : Base
{

};
