#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSet>
#include <QStringList>
#include <QPixmap>
#include <climits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("전북대학교 캠퍼스 길찾기");
    resize(750, 700);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);

    QLabel *title = new QLabel("<h2>전북대학교 캠퍼스 최단 경로 안내</h2>");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // 캠퍼스 사진
    QLabel *imageLabel = new QLabel;

    QPixmap pixmap("../../campus.jpg");

    if(!pixmap.isNull())
    {
        imageLabel->setPixmap(
            pixmap.scaled(
                650,
                250,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation));
    }
    else
    {
        imageLabel->setText("campus.jpg 이미지를 찾을 수 없습니다.");
    }

    imageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(imageLabel);

    startBox = new QComboBox;
    endBox = new QComboBox;

    QStringList places = {
        "공대7호관",
        "공대8호관",
        "학생회관",
        "중앙도서관",
        "진수당",
        "구정문",
        "후문",
        "운동장"
    };

    startBox->addItems(places);
    endBox->addItems(places);

    layout->addWidget(new QLabel("출발지 선택"));
    layout->addWidget(startBox);

    layout->addWidget(new QLabel("도착지 선택"));
    layout->addWidget(endBox);

    QPushButton *btn = new QPushButton("길찾기");
    layout->addWidget(btn);

    resultBox = new QTextEdit;
    resultBox->setReadOnly(true);
    layout->addWidget(resultBox);

    makeGraph();

    connect(btn, &QPushButton::clicked,
            this,
            [this]()
            {
                findPath();
            });
}

MainWindow::~MainWindow()
{
}

void MainWindow::makeGraph()
{
    graph["공대7호관"] = {
        {"공대8호관",120},
        {"학생회관",150}
    };

    graph["공대8호관"] = {
        {"공대7호관",120},
        {"운동장",200}
    };

    graph["학생회관"] = {
        {"공대7호관",150},
        {"중앙도서관",200},
        {"구정문",180}
    };

    graph["중앙도서관"] = {
        {"학생회관",200},
        {"진수당",100}
    };

    graph["진수당"] = {
        {"중앙도서관",100},
        {"후문",250}
    };

    graph["구정문"] = {
        {"학생회관",180},
        {"운동장",170}
    };

    graph["운동장"] = {
        {"공대8호관",200},
        {"구정문",170},
        {"후문",220}
    };

    graph["후문"] = {
        {"진수당",250},
        {"운동장",220}
    };
}

void MainWindow::findPath()
{
    QString start = startBox->currentText();
    QString end = endBox->currentText();

    if(start == end)
    {
        resultBox->setText("출발지와 도착지가 같습니다.");
        return;
    }

    QMap<QString,int> dist;
    QMap<QString,QString> parent;
    QSet<QString> visited;

    for(QString node : graph.keys())
        dist[node] = INT_MAX;

    dist[start] = 0;

    while(visited.size() < graph.size())
    {
        QString current;
        int minDist = INT_MAX;

        for(QString node : graph.keys())
        {
            if(!visited.contains(node)
                && dist[node] < minDist)
            {
                minDist = dist[node];
                current = node;
            }
        }

        if(current.isEmpty())
            break;

        visited.insert(current);

        for(auto edge : graph[current])
        {
            QString next = edge.first;
            int weight = edge.second;

            if(dist[current] + weight < dist[next])
            {
                dist[next] = dist[current] + weight;
                parent[next] = current;
            }
        }
    }

    if(dist[end] == INT_MAX)
    {
        resultBox->setText("경로를 찾을 수 없습니다.");
        return;
    }

    QList<QString> path;
    QString current = end;

    while(current != start)
    {
        path.prepend(current);
        current = parent[current];
    }

    path.prepend(start);

    QString result = "최단 경로\n\n";

    for(int i=0;i<path.size();i++)
    {
        result += path[i];

        if(i != path.size()-1)
            result += " -> ";
    }

    result += "\n\n총 이동 거리 : ";
    result += QString::number(dist[end]);
    result += " m";

    result += "\n이동 횟수 : ";
    result += QString::number(path.size()-1);

    resultBox->setText(result);
}