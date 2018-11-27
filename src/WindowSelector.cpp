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
#include "WindowSelector.h"
#include "PlatformDependent.h"
#include <iostream>
#include <string>
#include <vector>

WindowSelector::WindowSelector() : QDialog{}, Ui::WindowSelector{}
{
    setupUi(this);
    mWindowTreeView->setModel(&mWindowModel);
    refreshWindowList();
}

WindowSelector::~WindowSelector() = default;

void WindowSelector::refreshWindowList()
{
    std::vector<void *> window_id_list = GetTopLevelWindows();
    std::cout << "Found " << window_id_list.size() << " Top Level Windows." << std::endl;
    mWindowModel.FillWindows(window_id_list);
}

void WindowSelector::applyWindowChanges()
{
    QModelIndex index = mWindowTreeView->currentIndex();
    if (index.isValid())
    {
        SetWindowPosition(index.internalPointer(),
                          mXPositionSpinBox->value(),
                          mYPositionSpinBox->value(),
                          mWidthSpinBox->value(),
                          mHeightSpinBox->value());
    }
}
