#include "ui/window_variable.h"
#include "ui_window_variable.h"

WindowVariable::WindowVariable(QSharedPointer<Variable> var, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowVariable),
    m_var(var)
{
    ui->setupUi(this);
    if(m_var) {
        ui->name->setText(m_var->name());
        ui->id->setText(m_var->id());
        setWindowTitle("Edit variable: " + m_var->name());
        setWindowIcon(QIcon(":/icons/variables/tag_blue_edit.png"));
    } else {
        setWindowTitle("Add new variable");
        setWindowIcon(QIcon(":/icons/variables/tag_blue_add.png"));
    }
}

WindowVariable::~WindowVariable()
{
    delete ui;
}

QString WindowVariable::getName() const
{
    return ui->name->text();
}

QString WindowVariable::getId() const
{
    return ui->id->text();
}

void WindowVariable::on_ok_clicked()
{
    accept();
}

void WindowVariable::on_cancel_clicked()
{
    reject();
}

void WindowVariable::on_name_textChanged(const QString &name)
{
    ui->ok->setEnabled(!name.isEmpty());
}
