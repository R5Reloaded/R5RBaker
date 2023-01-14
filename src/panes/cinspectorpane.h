#ifndef CPROPERTIESPANE_H
#define CPROPERTIESPANE_H

#include <QWidget>

class CInspectorPane : public QWidget
{
    Q_OBJECT
public:
    class QObject* CurrentObject;

    class QVBoxLayout* vbox;

    class QLabel* TitleLabel;
    class QLabel* SubtitleLabel;

    class QPushButton* MenuButton;

    class QTableView* Table;
    class QStandardItemModel* Model;

    explicit CInspectorPane(QWidget *parent = nullptr);

    void extracted();
    void setInspectedObject(QObject *object);

private:
    void updateObjectProperty(const QModelIndex &index, const QModelIndex &);
    void setInspectorEnabled(bool enabled);

signals:

};

#endif // CPROPERTIESPANE_H
