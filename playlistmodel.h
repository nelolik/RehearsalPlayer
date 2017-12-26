#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QList>
#include <QStringListModel>

class QVariant;
class QStringList;
class QItemSelectionModel;
class MediaItem;

class PlaylistModel : public QAbstractListModel
{
public:
    PlaylistModel(QList<MediaItem> *playlist, QObject * parent = 0);

    // Basic required information
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    Qt::DropActions supportedDragActions() const override {return Qt::MoveAction;}
    Qt::DropActions supportedDropActions() const override {return Qt::MoveAction;}
    QStringList mimeTypes() const override { return QStringList() << MimeType;}
    QMimeData* mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    //Not overloaded functions
    void clearContainer();
    void appendItems(QList<MediaItem>&);
    void changedData(int top, int bottom);

public slots:
    void onDelButton(QItemSelectionModel *selectionModel);


private:
    QList<MediaItem> *m_container;
    const QString MimeType = "application/rhpl.pls.bin";
};

#endif // PLAYLISTMODEL_H
