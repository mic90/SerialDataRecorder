#ifndef WINDOW_VARIABLE_H
#define WINDOW_VARIABLE_H

#include <QDialog>
#include <project/channel.h>

namespace Ui {
class WindowVariable;
}

class WindowVariable : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(WindowVariable)
public:
    explicit WindowVariable(QWidget *parent = nullptr);
    WindowVariable(Channel const& var, QWidget *parent = nullptr);
    ~WindowVariable();

    QString getName() const;
    QString getUnit() const;
    QString getId() const;

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();
    void on_name_textChanged(const QString &arg1);
    void on_id_textChanged(const QString &arg1);

private:
    Ui::WindowVariable *ui;
};

#endif // WINDOW_VARIABLE_H
