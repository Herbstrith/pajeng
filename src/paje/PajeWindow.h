#include <QtGui>
#include <QMainWindow>
#include "PajeSpaceTimeFrame.h"
#include "PajeTreemapFrame.h"

QT_FORWARD_DECLARE_CLASS(QSplitter)
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QMenu)

class PajeWindow : public QMainWindow
{
  Q_OBJECT;

private:
  static PajeWindow *instance;
public:
  PajeSpaceTimeFrame *spacetimeFrame;
  PajeTreemapFrame *treemapFrame;

  static PajeWindow *getInstance() {
    if (!instance){
      instance = new PajeWindow ();
    }
    return instance;
  };

protected:
  PajeWindow();
  void closeEvent(QCloseEvent *event);

private slots:
  void about();

private:
  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void readSettings();
  void writeSettings();

  QMenu *fileMenu;
  QMenu *helpMenu;
  QAction *exitAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
};