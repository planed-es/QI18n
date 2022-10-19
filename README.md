# QI18n

QI18n is an internationalization library for the [Qt framework](https://qt.io), using JSON files
to load your translations.

# How to use
### Initialization

QI18n must be initialized before it is used. You may instantiate it in your `main` function, to make
it available from all your application:

```
#include <QApplication>
#include <qi18n.h>

int main(int argc, char** argv)
{
  QApplication application(argc, argv);
  QI18n i18n(":/locales", "en"); 

  return application.exec();
}
```

The QI18n constructor takes at least two parameters:

* A root path in which your JSON translations files will be looked up for
* A default locale, which will be pre-loaded and used until you explicitely set another locale

In this example, we use `":/locales"` as the root path, meaning translation files will be looked up using
the [Qt Resource System](https://doc.qt.io/qt-6/resources.html), in the `locales` folder. As
for the default locale, it is set to `"en"`, meaning translations will be loaded from all the
JSON files ending with the `en.json` pattern.

### Translating text

Translations are defined in JSON files presented as such:

```JSON
{
  "hello": "Hello world !"
}
```

You can then access the translation from your code using:

```c++
QI18n* i18n = QI18n::get();

qDebug() << i18n->t("hello");
```

### Categories

You can also use JSON sub-objects to sort your translations:

```JSON
{
  "category": {
    "message": "Hello category !"
  }
}
```

Which is then used like this:

```c++
i18n->t("category.message");
```

### Extrapolation

Extrapolation can be defined in your translations using the `{{}}` delimiters, such as:

```JSON
{
  "hello": "Hello {{name}} !"
}
```

The values are then provided using a QVariantMap:

```c++
i18n->t("hello", QVariantMap{{"name", "John"}});
```
