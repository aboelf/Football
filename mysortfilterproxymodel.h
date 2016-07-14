#ifndef MYSORTFILTERPROXYMODEL_H
#define MYSORTFILTERPROXYMODEL_H
#include <QSortFilterProxyModel>

class MySortFilterProxyModel : public QSortFilterProxyModel
{
public:
    MySortFilterProxyModel(int col);

    double fileterMinimumValue() const {return minValue;}
    void setFilterMinimumValue(const double &value);

    double filterMaximumValue() const {return maxValue;}
    void setFilterMaximumValue(const double &value);

protected:
    bool filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const Q_DECL_OVERRIDE;
//    bool moreThan(const QModelIndex &left, const QModelIndex &right) const Q_DECL_OVERRIDE;
private:
    bool ValueInRange(const double &value) const;

    double minValue;
    double maxValue;

    int column;
};

#endif // MYSORTFILTERPROXYMODEL_H
