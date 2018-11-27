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
#include <array>
#include <cassert>

#include "PlatformDependent.h"
#include "WindowModel.h"

WindowModel::WindowModel(QObject *parent) : QAbstractItemModel(parent) {}

WindowModel::~WindowModel() = default;

QVariant WindowModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
    {
        switch (section)
        {
        case 0:
            return QString("Id");
        case 1:
            return QString("Class");
        case 2:
            return QString("Title");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QModelIndex WindowModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < static_cast<int>(mWindows.size()))
    {
        return createIndex(row, column, std::get<0>(mWindows[row]));
    }
    return QModelIndex();
}

QModelIndex WindowModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int WindowModel::rowCount(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return static_cast<int>(mWindows.size());
    }
    // Only root may have children/rows
    return 0;
}

int WindowModel::columnCount(const QModelIndex &index) const
{
    return 3;
}

bool WindowModel::hasChildren(const QModelIndex &index) const
{
    return rowCount() > 0;
}

QVariant WindowModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if (role == Qt::EditRole || role == Qt::DisplayRole)
            switch (index.column())
            {
            case 0:
                if (role == Qt::DisplayRole)
                {
                    return QString("0x%1").arg((quintptr)std::get<0>(mWindows[index.row()]), QT_POINTER_SIZE * 2, 16, QChar('0'));
                }
                break;
            case 1:
                return QString::fromStdString(std::get<1>(mWindows[index.row()]));
            case 2:
                return QString::fromStdString(std::get<2>(mWindows[index.row()]));
            }
    }
    return QVariant();
}

Qt::ItemFlags WindowModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

bool WindowModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

static bool CompareAscendingIds(const WindowModel::Window &a, const WindowModel::Window &b)
{
    return std::get<0>(a) < std::get<0>(b);
}

static bool CompareAscendingClasses(const WindowModel::Window &a, const WindowModel::Window &b)
{
    return std::get<1>(a) < std::get<1>(b);
}

static bool CompareAscendingTitles(const WindowModel::Window &a, const WindowModel::Window &b)
{
    return std::get<2>(a) < std::get<2>(b);
}

static std::array<bool (*)(const WindowModel::Window &a, const WindowModel::Window &b), 3> CompareAscending{
    CompareAscendingIds, CompareAscendingClasses, CompareAscendingTitles};

static bool CompareDescendingIds(const WindowModel::Window &a, const WindowModel::Window &b)
{
    return std::get<0>(a) > std::get<0>(b);
}

static bool CompareDescendingClasses(const WindowModel::Window &a, const WindowModel::Window &b)
{
    return std::get<1>(a) > std::get<1>(b);
}

static bool CompareDescendingTitles(const WindowModel::Window &a, const WindowModel::Window &b)
{
    return std::get<2>(a) > std::get<2>(b);
}

static std::array<bool (*)(const WindowModel::Window &a, const WindowModel::Window &b), 3> CompareDescending{
    CompareDescendingIds, CompareDescendingClasses, CompareDescendingTitles};

void WindowModel::sort(int column, Qt::SortOrder order)
{
    beginResetModel();
    if (order == Qt::AscendingOrder)
    {
        std::sort(mWindows.begin(), mWindows.end(), CompareAscending[column]);
    }
    else
    {
        std::sort(mWindows.begin(), mWindows.end(), CompareDescending[column]);
    }
    endResetModel();
}

void WindowModel::FillWindows(const std::vector<void *> &aWindowVector)
{
    beginResetModel();
    mWindows.clear();
    mWindows.reserve(aWindowVector.size());
    for (const auto &i : aWindowVector)
    {
        mWindows.emplace_back(i, GetWindowClass(i), GetWindowTitle(i));
    }
    endResetModel();
}
