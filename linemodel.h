#ifndef LINEMODEL_H
#define LINEMODEL_H

#include <QAbstractListModel>

class Line;
class LineModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Line *mline READ mline WRITE setMline)
public:
    explicit LineModel(QObject *parent = nullptr);

    enum{
        B1 = Qt::UserRole,
        B2,B3,B4,B5,B6,B7,B8,B9,
        B10,B11,B12,B13,B14,B15,B16,
        typeRole,gestureRole,boldRole,
        directionRole,leftRole,rightRole,
        turingPointRole,previousRole,
        buttonHeightRole
    };
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;
    Line *mline() const;
    void setMline(Line *mline);
private:
    Line *mLine;
};

#endif // LINEMODEL_H
