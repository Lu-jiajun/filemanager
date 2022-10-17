#ifndef MYFILESYTEMMODEL_H
#define MYFILESYTEMMODEL_H


#include <QFileSystemModel>
#include <QTreeView>

class MyFileSytemModel : public QFileSystemModel
{
public:
    //第1列显示名称，其他几列都屏蔽掉
    explicit MyFileSytemModel(QObject *parent = nullptr);
    int columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return 1;
    }
};

#endif // MYFILESYTEMMODEL_H
