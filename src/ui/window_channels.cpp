#include "ui/window_channels.h"
#include "ui_window_channels.h"
#include <QDebug>

WindowChannels::WindowChannels(QList<Channel> channels, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowChannels),
    m_channelsCount(0),
    m_channels(channels)
{
    ui->setupUi(this);
    ui->channelsCount->setValue(m_channels.size());
}

WindowChannels::~WindowChannels()
{
    delete ui;
}

QList<Channel> WindowChannels::getChannels() const
{
    QList<Channel> channels;
    int index = 0;
    for(auto item : m_items)
    {
        channels.append(Channel(item->text(), index++));
    }
    return channels;
}

void WindowChannels::setChannelsCount(int count)
{
    while(m_items.size() > 0)
    {
        delete m_items.takeFirst();
    }

    for(int i=0; i<count; i++)
    {
        QString name = (i < m_channels.size()) ? m_channels.at(i).name() : QString("Channel%1").arg(i + 1);
        QListWidgetItem *newItem = new QListWidgetItem(name, ui->channels);
        newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
        newItem->setIcon(QIcon(":/icons/variables/tag_blue.png"));
        m_items.append(newItem);
    }
}

void WindowChannels::on_channelsCount_valueChanged(int count)
{
    m_channelsCount = count;
    setChannelsCount(m_channelsCount);
}

void WindowChannels::on_ok_clicked()
{
    accept();
}

void WindowChannels::on_cancel_clicked()
{
    reject();
}
