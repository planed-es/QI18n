#ifndef  QI18N_H
# define QI18N_H

# include "QI18n_global.h"
# include <QObject>
# include <QJsonObject>
# include <QVariantMap>

class QI18N_EXPORT QI18n : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString currentLocale MEMBER currentLocale NOTIFY currentLocaleChanged)

  static QI18n* instance;
public:
  explicit QI18n(const QString& rootPath, const QString& defaultLocale, QObject *parent = nullptr);

  static QI18n* get() { return instance; }

  Q_INVOKABLE QStringList getAvailableLocales() const { return locales; }
  Q_INVOKABLE QString t(const QString& key) const;
  Q_INVOKABLE QString t(const QString& key, const QVariantMap& variables) const;
  const QString& getCurrentLocale() const { return currentLocale; }

  static QString getSourceForLocale(const QString& locale);
  static QString getSourceForLocale(const QString& translationFile, const QString& locale);

signals:
  void currentLocaleChanged();
  void translationsChanged();

public slots:
  void loadCurrentLocale();

private:
  QJsonObject getTranslationGroupForKey(const QString& key) const;
  QJsonObject getTranslationGroup(const QStringList& path) const;

  const QString rootPath;
  QString       currentLocale;
  QStringList   locales;
  QJsonObject   data;
};

#endif // QI18N_H
