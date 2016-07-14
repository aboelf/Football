#include "mysortfilterproxymodel.h"
#include <QVariant>
#include <QModelIndex>
MySortFilterProxyModel::MySortFilterProxyModel(int col):QSortFilterProxyModel()
{
    column=col;
}

void MySortFilterProxyModel::setFilterMinimumValue(const double &value)
{
    minValue=value;
    invalidateFilter();
}

void MySortFilterProxyModel::setFilterMaximumValue(const double &value)
{
    maxValue=value;
    invalidateFilter();
}

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index=sourceModel()->index(sourceRow,column,sourceParent);
    return ValueInRange(sourceModel()->data(index).toDouble());
}

bool MySortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant leftDate=sourceModel()->data(left);
    QVariant rightDate=sourceModel()->data(right);

    if(leftDate.type()==QVariant::Double)
    {
        return leftDate.toDouble()<rightDate.toDouble();
    }
//    else
//    {

    //    }
}

bool MySortFilterProxyModel::ValueInRange(const double &value) const
{
    return (value>=minValue) && (value<=maxValue);
}


