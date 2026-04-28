#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include "compiler_engine.h"

class FinalCompiler : public QMainWindow {
public:
    FinalCompiler() {
        QWidget *central = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(central);

        editor = new QTextEdit();
        editor->setPlaceholderText("// Test 6-Phases:\nint a = 10 + 20;\nfloat b[5];\nb[10] = 5;");
        editor->setStyleSheet("background-color: #1e1e1e; color: #ffffff; font-family: 'Consolas'; font-size: 12pt;");

        QPushButton *runBtn = new QPushButton("🚀 RUN 6-PHASE ANALYSIS");
        runBtn->setStyleSheet("background-color: #2ecc71; color: white; font-weight: bold; padding: 12px;");

        tabs = new QTabWidget();
        errorTable = new QTableWidget(0, 4);
        errorTable->setHorizontalHeaderLabels({"Line", "Phase", "Message", "Recovery"});
        
        symbolView = new QTableWidget(0, 2);
        symbolView->setHorizontalHeaderLabels({"Variable", "Type"});

        icgView = new QTextEdit(); // New Tab for ICG
        icgView->setReadOnly(true);
        icgView->setStyleSheet("background-color: #2d2d2d; color: #00ff00; font-family: monospace;");

        scene = new QGraphicsScene();
        treeDisplay = new QGraphicsView(scene);
        treeDisplay->setBackgroundBrush(QBrush(QColor(30, 30, 30)));

        tabs->addTab(errorTable, "Errors");
        tabs->addTab(symbolView, "Symbol Table");
        tabs->addTab(icgView, "Intermediate Code (ICG)");
        tabs->addTab(treeDisplay, "Parse Tree");

        layout->addWidget(editor);
        layout->addWidget(runBtn);
        layout->addWidget(tabs);

        setCentralWidget(central);
        setWindowTitle("Mini Compiler 6-Phase Toolchain");
        resize(1100, 800);

        connect(runBtn, &QPushButton::clicked, this, &FinalCompiler::onRun);
    }

    void drawTree(Node* node, int x, int y, int xOffset) {
        if (!node) return;
        scene->addEllipse(x, y, 100, 40, QPen(Qt::white), QBrush(QColor(70, 130, 180)));
        QGraphicsTextItem* text = scene->addText(QString::fromStdString(node->name));
        text->setDefaultTextColor(Qt::white);
        text->setPos(x + 5, y + 8);

        int nextX = x - (xOffset * (int)(node->children.size() - 1)) / 2;
        for (Node* child : node->children) {
            scene->addLine(x + 50, y + 40, nextX + 50, y + 100, QPen(Qt::lightGray));
            drawTree(child, nextX, y + 100, xOffset / 1.5);
            nextX += xOffset;
        }
    }

    void onRun() {
        CompilerEngine engine;
        auto results = engine.analyze(editor->toPlainText().toStdString());

        // Fill Errors
        errorTable->setRowCount(0);
        for(auto &e : results) {
            int r = errorTable->rowCount();
            errorTable->insertRow(r);
            errorTable->setItem(r, 0, new QTableWidgetItem(QString::number(e.line)));
            errorTable->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(e.type)));
            errorTable->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(e.message)));
            errorTable->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(e.recovery)));
        }

        // Fill Symbol Table
        symbolView->setRowCount(0);
        for(auto const& [var, type] : engine.symbolTable) {
            int r = symbolView->rowCount();
            symbolView->insertRow(r);
            symbolView->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(var)));
            symbolView->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(type)));
        }

        // Fill ICG (Three Address Code)
        icgView->clear();
        for(const auto& code : engine.intermediateCode) {
            icgView->append(QString::fromStdString(code));
        }

        scene->clear();
        drawTree(engine.root, 0, 0, 450);
    }

private:
    QTextEdit *editor, *icgView;
    QTabWidget *tabs;
    QTableWidget *errorTable, *symbolView;
    QGraphicsView *treeDisplay;
    QGraphicsScene *scene;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    FinalCompiler w;
    w.show();
    return a.exec();
}
