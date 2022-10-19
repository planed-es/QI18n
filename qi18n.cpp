#include "qi18n.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QSettings>
#include <QDir>
#include <QDebug>

QI18n* QI18n::instance = nullptr;

static void merge(QJsonObject& target, const QJsonObject& source)
{
  for (auto it = source.constBegin() ; it != source.constEnd() ; ++it)
  {
    QJsonObject::iterator target_iterator = target.find(it.key());

    if (target_iterator != target.end())
    {
      if (target_iterator->isObject() && it->isObject())
      {
        QJsonObject newObject = target_iterator->toObject();

        merge(newObject, it->toObject());
        target.insert(it.key(), newObject);
      }
      else
        qDebug() << "QI18n: merge(QJsonObject,QJsonObject): duplicate key cannot be merged:" << it.key();
    }
    else
      target.insert(it.key(), it.value());
  }
}

QI18n::QI18n(const QString& rootPath, const QString& defaultLocaleName, QObject *parent) : QObject(parent), rootPath(rootPath)
{
  QString defaultLocale = QSettings().value("locale", defaultLocaleName).toString();

  instance = this;
  QStringList files = QDir(rootPath).entryList(QStringList() << "*.json", QDir::NoFilter, QDir::Name);
  for (QString& file : files)
    locales << file.replace(".json", "");
  if (locales.contains(defaultLocale))
    currentLocale = defaultLocale;
  else if (locales.size() > 0)
    currentLocale = locales.first();
  loadCurrentLocale();
  connect(this, &QI18n::currentLocaleChanged, this, &QI18n::loadCurrentLocale);
}

QString QI18n::getSourceForLocale(const QString &locale)
{
  return QI18n::get()->rootPath + '/' + locale + ".json";
}

QString QI18n::getSourceForLocale(const QString& translationFile, const QString &locale)
{
  QI18n* i18n = QI18n::get();
  const auto base_path = i18n->rootPath + '/' + translationFile;

  if (*translationFile.rbegin() == '/')
    return base_path + locale + ".json";
  return base_path + '.' + locale + ".json";
}

static QStringList getSourcesForLocale(const QString& locale, const QString& basePath)
{
  QDir directory(basePath);
  QString filter = "*." + locale + ".json";
  QFileInfoList list;
  QStringList files;
  QStringList bannedFolder{".", ".."};

  directory.setFilter(QDir::Dirs);
  list = directory.entryInfoList();
  for (int i = 0 ; i < list.size() ; ++i)
  {
    if (bannedFolder.indexOf(list.at(i).fileName()) >= 0) continue ;
    files << getSourcesForLocale(locale, basePath + '/' + list.at(i).fileName() + '/');
  }
  directory.setNameFilters(QStringList() << filter << (locale + ".json"));
  directory.setFilter(QDir::Files);
  list = directory.entryInfoList();
  for (int i = 0 ; i < list.size() ; ++i)
    files << (basePath + '/' + list.at(i).fileName());
  return files;
}

static void loadLocale(const QString& locale, QJsonObject& data, const QString& rootPath)
{
  while (data.begin() != data.end()) data.erase(data.begin());
  for (const QString& filename : getSourcesForLocale(locale, rootPath))
  {
    QFile file(filename);

    qDebug() << "QI18n: Loading file" << filename;
    if (file.open(QIODevice::ReadOnly))
    {
      QJsonObject fileData = QJsonDocument::fromJson(file.readAll()).object();

      merge(data, fileData);
    }
    else
      qDebug() << "QI18n: failed to open file" << filename;
  }
}

void QI18n::loadCurrentLocale()
{
  qDebug() << "QI18n: Loading locale" << currentLocale;
  loadLocale(currentLocale, data, rootPath);
  QSettings().setValue("locale", currentLocale);
  emit translationsChanged();
}

QString QI18n::t(const QString &key) const
{
  QJsonObject group = getTranslationGroupForKey(key);
  QJsonValue value = group[key.split('.').last()];

  if (value.isUndefined() || value.isNull())
    return key;
  return value.toString();
}

QString QI18n::t(const QString& key, const QVariantMap& variables) const
{
  QString str = t(key);

  for (const QString& varname : variables.keys())
  {
    QString hint("{{" + varname + "}}");

    str.replace(hint, variables[varname].toString());
  }
  return str;
}

QJsonObject QI18n::getTranslationGroupForKey(const QString &key) const
{
  if (key.indexOf('.') > 0)
  {
    QStringList keys = key.split('.');

    keys.removeLast();
    return getTranslationGroup(keys);
  }
  return data;
}

QJsonObject QI18n::getTranslationGroup(const QStringList &path) const
{
  QJsonObject group = data;

  for (const QString& key : path)
    group = group[key].toObject();
  return group;
}
