#include "regullabel.h"
#include <QDebug>

RegulLabel::RegulLabel(const QString &texte, QWidget *parent) : QLabel(parent)
{
    setStyleSheet("RegulLabel{border-style: solid; border-radius: 25px; border-width: 1px; border-color: #8C87D7} RegulLabel:hover{border: 1px solid red}");
    setMargin(10);
    setMaximumHeight(50);
    setAlignment(Qt::AlignCenter);
    setTextFormat(Qt::PlainText);
    QFont font( "HanziPen TC", 14, QFont::Bold);
    setFont(font);
    setText(texte);
}

void RegulLabel::mousePressEvent(QMouseEvent */*event*/)
{
    emit clicked();
}
