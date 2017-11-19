#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QList>
#include <QStringListModel>

class QVariant;
class QStringList;
class QItemSelectionModel;

class PlaylistModel : public QAbstractListModel
{
public:
    PlaylistModel(QList<QStringList> *playlist, QObject * parent = 0);

    // Basic required information
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;

    //Not overloaded functions
    void clearContainer();
    void appendItems(QList<QStringList>&);

public slots:
    void onDelButton(QItemSelectionModel *selectionModel);


private:
    QList<QStringList> *m_container;
};

#endif // PLAYLISTMODEL_H
