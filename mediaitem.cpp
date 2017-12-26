#include "mediaitem.h"
#include <QByteArray>

MediaItem::MediaItem() :
    fileName(""),
    filePath(""),
    timeLables(),
    isPlaying(false)
{

}

MediaItem::MediaItem(QString file_name, QString file_path) :
    fileName(file_name),
    filePath(file_path),
    timeLables(),
    isPlaying(false)
{

}

MediaItem::MediaItem(QString file_name, QString file_path, bool is_playing, QList<int> &labels) :
    fileName(file_name),
    filePath(file_path),
    timeLables(labels),
    isPlaying(is_playing)
{

}

QByteArray MediaItem::serialize()
{
    QByteArray binData;
    QDataStream out(&binData, QIODevice::WriteOnly);
    int n_labels = timeLables.size();
    out << fileName << filePath << isPlaying << n_labels;
    foreach (int time, timeLables) {
        out << time;
    }
    return binData;
}

MediaItem MediaItem::deserialize(QByteArray &binData)
{
    QDataStream in(binData);
    QString f_name, f_path;
    bool is_playing;
    int n_labels;
    QList<int> labels;
    in >> f_name >> f_path>> is_playing >> n_labels;
    for (int i = 0; i < n_labels; ++i) {
        in >> labels;
    }
    return MediaItem(f_name, f_path, is_playing, labels);
}
