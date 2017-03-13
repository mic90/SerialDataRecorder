#ifndef WINDOW_VARIABLE_H
#define WINDOW_VARIABLE_H

#include <QDialog>
#include <project/variable.h>

namespace Ui {
class WindowVariable;
}

class WindowVariable : public QDialog
{
    Q_OBJECT

public:
    WindowVariable(QSharedPointer<Variable> var = QSharedPointer<Variable>(nullptr), QWidget *parent = nullptr);
    ~WindowVariable();

    QString getName() const;
    QString getId() const;

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();
    void on_name_textChanged(const QString &arg1);

private:
    Ui::WindowVariable *ui;
    QSharedPointer<Variable> m_var;
};

#endif // WINDOW_VARIABLE_H
