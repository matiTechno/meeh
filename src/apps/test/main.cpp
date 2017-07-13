#include <meeh/Main.hpp>
#include <meeh/Scene.hpp>
#include <stdio.h>

int main()
{
    try
    {
        meeh::Main main;
        main.start(std::make_unique<meeh::Scene>());
    }
    catch(const std::exception& e)
    {printf("%s\n", e.what());}
    return 0;
}
