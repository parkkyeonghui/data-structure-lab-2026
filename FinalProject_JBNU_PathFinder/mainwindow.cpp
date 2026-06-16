#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSet>
#include <QStringList>
#include <QPixmap>
#include <climits>

// MainWindow 생성자
// 프로그램의 메인 GUI 화면을 구성하고, 버튼 클릭 이벤트를 연결한다.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 프로그램 창 제목과 크기 설정
    setWindowTitle("전북대학교 캠퍼스 길찾기");
    resize(750, 700);

    // QMainWindow에 들어갈 중앙 위젯 생성
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // 위젯들을 세로 방향으로 배치하기 위한 레이아웃 생성
    QVBoxLayout *layout = new QVBoxLayout(central);

    // 프로그램 제목 라벨 생성
    QLabel *title = new QLabel("<h2>전북대학교 캠퍼스 최단 경로 안내</h2>");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // 캠퍼스 지도 이미지를 출력하기 위한 라벨 생성
    QLabel *imageLabel = new QLabel;

    // campus.jpg 파일을 불러온다.
    // 실행 파일 기준 상대 경로를 사용하기 위해 ../../campus.jpg로 지정하였다.
    QPixmap pixmap("../../campus.jpg");

    // 이미지 파일을 정상적으로 불러온 경우 크기를 조절하여 화면에 출력한다.
    if(!pixmap.isNull())
    {
        imageLabel->setPixmap(
            pixmap.scaled(
                650,
                250,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation));
    }
    // 이미지 파일을 찾지 못한 경우 안내 문구를 출력한다.
    else
    {
        imageLabel->setText("campus.jpg 이미지를 찾을 수 없습니다.");
    }

    imageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(imageLabel);

    // 출발지와 도착지를 선택하기 위한 콤보박스 생성
    startBox = new QComboBox;
    endBox = new QComboBox;

    // 그래프의 정점에 해당하는 캠퍼스 주요 건물 목록
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

    // 콤보박스에 건물 목록 추가
    startBox->addItems(places);
    endBox->addItems(places);

    // 출발지 선택 영역 추가
    layout->addWidget(new QLabel("출발지 선택"));
    layout->addWidget(startBox);

    // 도착지 선택 영역 추가
    layout->addWidget(new QLabel("도착지 선택"));
    layout->addWidget(endBox);

    // 길찾기 버튼 생성
    QPushButton *btn = new QPushButton("길찾기");
    layout->addWidget(btn);

    // 최단 경로 결과를 출력할 텍스트 박스 생성
    resultBox = new QTextEdit;
    resultBox->setReadOnly(true);
    layout->addWidget(resultBox);

    // 캠퍼스 건물 간 연결 관계를 그래프로 저장
    makeGraph();

    // 길찾기 버튼을 클릭하면 findPath() 함수가 실행되도록 연결
    connect(btn, &QPushButton::clicked,
            this,
            [this]()
            {
                findPath();
            });
}

// 소멸자
// 동적으로 생성한 Qt 위젯들은 부모 객체에 의해 자동으로 정리된다.
MainWindow::~MainWindow()
{
}

// 캠퍼스 건물 간 연결 관계와 이동 거리를 그래프에 저장하는 함수
// 건물은 정점(Vertex), 건물 사이의 길은 간선(Edge), 이동 거리는 가중치(Weight)에 해당한다.
void MainWindow::makeGraph()
{
    // 공대7호관과 연결된 건물 및 거리
    graph["공대7호관"] = {
        {"공대8호관",120},
        {"학생회관",150}
    };

    // 공대8호관과 연결된 건물 및 거리
    graph["공대8호관"] = {
        {"공대7호관",120},
        {"운동장",200}
    };

    // 학생회관과 연결된 건물 및 거리
    graph["학생회관"] = {
        {"공대7호관",150},
        {"중앙도서관",200},
        {"구정문",180}
    };

    // 중앙도서관과 연결된 건물 및 거리
    graph["중앙도서관"] = {
        {"학생회관",200},
        {"진수당",100}
    };

    // 진수당과 연결된 건물 및 거리
    graph["진수당"] = {
        {"중앙도서관",100},
        {"후문",250}
    };

    // 구정문과 연결된 건물 및 거리
    graph["구정문"] = {
        {"학생회관",180},
        {"운동장",170}
    };

    // 운동장과 연결된 건물 및 거리
    graph["운동장"] = {
        {"공대8호관",200},
        {"구정문",170},
        {"후문",220}
    };

    // 후문과 연결된 건물 및 거리
    graph["후문"] = {
        {"진수당",250},
        {"운동장",220}
    };
}

// 사용자가 선택한 출발지와 도착지 사이의 최단 경로를 찾는 함수
// 건물 간 이동 거리가 서로 다르기 때문에 다익스트라 알고리즘을 사용한다.
void MainWindow::findPath()
{
    // 콤보박스에서 사용자가 선택한 출발지와 도착지 가져오기
    QString start = startBox->currentText();
    QString end = endBox->currentText();

    // 출발지와 도착지가 같은 경우 예외 처리
    if(start == end)
    {
        resultBox->setText("출발지와 도착지가 같습니다.");
        return;
    }

    // dist: 시작 정점에서 각 정점까지의 최단 거리 저장
    // parent: 최단 경로를 복원하기 위해 이전 정점 저장
    // visited: 최단 거리가 확정된 정점 저장
    QMap<QString,int> dist;
    QMap<QString,QString> parent;
    QSet<QString> visited;

    // 모든 정점의 거리를 무한대로 초기화
    for(QString node : graph.keys())
        dist[node] = INT_MAX;

    // 시작 정점의 거리는 0으로 설정
    dist[start] = 0;

    // 모든 정점을 확인할 때까지 반복
    while(visited.size() < graph.size())
    {
        QString current;
        int minDist = INT_MAX;

        // 아직 방문하지 않은 정점 중 거리가 가장 짧은 정점을 선택
        for(QString node : graph.keys())
        {
            if(!visited.contains(node)
                && dist[node] < minDist)
            {
                minDist = dist[node];
                current = node;
            }
        }

        // 더 이상 이동할 수 있는 정점이 없으면 반복 종료
        if(current.isEmpty())
            break;

        // 선택된 정점을 방문 처리
        visited.insert(current);

        // 현재 정점과 연결된 모든 이웃 정점을 확인
        for(auto edge : graph[current])
        {
            QString next = edge.first;     // 연결된 다음 건물
            int weight = edge.second;      // 현재 건물에서 다음 건물까지의 거리

            // 현재 정점을 거쳐 가는 경로가 기존 경로보다 짧으면 최단 거리 갱신
            if(dist[current] + weight < dist[next])
            {
                dist[next] = dist[current] + weight;
                parent[next] = current;
            }
        }
    }

    // 도착지까지의 경로가 없는 경우
    if(dist[end] == INT_MAX)
    {
        resultBox->setText("경로를 찾을 수 없습니다.");
        return;
    }

    // parent 정보를 이용하여 도착지부터 출발지까지 역순으로 경로 복원
    QList<QString> path;
    QString current = end;

    while(current != start)
    {
        path.prepend(current);
        current = parent[current];
    }

    // 출발지를 경로 맨 앞에 추가
    path.prepend(start);

    // 결과 문자열 생성
    QString result = "최단 경로\n\n";

    // 최단 경로 출력 형식 생성
    for(int i=0;i<path.size();i++)
    {
        result += path[i];

        if(i != path.size()-1)
            result += " -> ";
    }

    // 총 이동 거리 출력
    result += "\n\n총 이동 거리 : ";
    result += QString::number(dist[end]);
    result += " m";

    // 이동 횟수 출력
    result += "\n이동 횟수 : ";
    result += QString::number(path.size()-1);

    // 결과 출력창에 최종 결과 표시
    resultBox->setText(result);
}