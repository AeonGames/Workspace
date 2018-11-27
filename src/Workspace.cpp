/*
Copyright (C) 2018 Rodrigo Jose Hernandez Cordoba

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Workspace.h"
#include "WindowSelector.h"
#include <iostream>

#if defined(__GNUC__) && ((__GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 8 || (__GNUC_MINOR__ == 8 && __GNUC_PATCHLEVEL__ > 4)))) || defined(__i386__))
#define ENTRYPOINT __attribute__((force_align_arg_pointer))
#else
#define ENTRYPOINT
#endif

int ENTRYPOINT main(int argc, char *argv[])
{
    int retval = 0;
    try
    {
        {
            QApplication workspace(argc, argv);
            workspace.setWindowIcon(QIcon(":/icons/Workspace"));
            workspace.setOrganizationName("AeonGames");
            workspace.setOrganizationDomain("aeongames.com");
            workspace.setApplicationName("Workspace");
            WindowSelector window_selector{};
            window_selector.showNormal();
            retval = workspace.exec();
        }
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        retval = -1;
    }
    return retval;
}
