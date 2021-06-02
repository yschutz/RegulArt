#include "regulpeintre.h"

///////////////////////////////////////////////////////////////////////////////////////////
RegulPeintre::RegulPeintre(const QString &nom, const QString &complet, QObject *parent) : QObject(parent),
    m_nom(nom), m_nomComplet(complet)
{

}

RegulPeintre::~RegulPeintre()
{
    qDeleteAll(m_tableaux.begin(), m_tableaux.end());
    m_tableaux.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////
void RegulPeintre::addTableau(RegulTableau *t)
{
  m_tableaux.append(t);
}
