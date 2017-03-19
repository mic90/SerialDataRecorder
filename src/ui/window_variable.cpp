#include "ui/window_variable.h"
#include "ui_window_variable.h"

WindowVariable::WindowVariable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowVariable)
{
    ui->setupUi(this);
    setWindowTitle("Add new variable");
    setWindowIcon(QIcon(":/icons/variables/tag_blue_add.png"));
}

WindowVariable::WindowVariable(const Channel &var, QWidget *parent) :
    WindowVariable(parent)
{
    ui->name->setText(var.name());
    ui->id->setText(QString::number(var.id()));
    setWindowTitle("Edit variable: " + var.name());
    setWindowIcon(QIcon(":/icons/variables/tag_blue_edit.png"));
}

WindowVariable::~WindowVariable()
{
    delete ui;
}

QString WindowVariable::getName() const
{
    return ui->name->text();
}

QString WindowVariable::getUnit() const
{
    return ui->unit->text();
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
    ui->ok->setEnabled(!name.isEmpty() && !ui->id->text().isEmpty());
}

void WindowVariable::on_id_textChanged(const QString &id)
{
    ui->ok->setEnabled(!id.isEmpty() && !ui->name->text().isEmpty());
}
