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

#include "PlatformDependent.h"
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static BOOL CALLBACK CountWindows(
    _In_ HWND hwnd,
    _In_ LPARAM lParam)
{
    size_t *window_count = reinterpret_cast<size_t *>(lParam);
    ++(*window_count);
    return TRUE;
}

static BOOL CALLBACK CollectWindowIds(
    _In_ HWND hwnd,
    _In_ LPARAM lParam)
{
    std::vector<void *> *window_id_vector = reinterpret_cast<std::vector<void *> *>(lParam);
    window_id_vector->push_back(hwnd);
    return TRUE;
}

std::vector<void *> GetTopLevelWindows()
{
    size_t count{0};
    std::vector<void *> result;

    if (!EnumWindows(CountWindows, LPARAM(&count)))
    {
        std::cout << "EnumWindows -> CountWindows failed." << std::endl;
    }
    result.reserve(count);
    if (!EnumWindows(CollectWindowIds, LPARAM(&result)))
    {
        std::cout << "EnumWindows -> CollectWindowIds failed." << std::endl;
    }
    return result;
}

std::string GetWindowTitle(void *aWindowId)
{
    char window_title[1024]{};
    GetWindowText(reinterpret_cast<HWND>(aWindowId), window_title, 1024);
    return std::string{window_title};
}

std::string GetWindowClass(void *aWindowId)
{
    char window_class[1024]{};
    GetClassName(reinterpret_cast<HWND>(aWindowId), window_class, 1024);
    return std::string{window_class};
}

void SetWindowPosition(void *aWindowId, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight)
{
    SetWindowPos(reinterpret_cast<HWND>(aWindowId), HWND_TOP, aX, aY, aWidth, aHeight, 0);
}

#endif
