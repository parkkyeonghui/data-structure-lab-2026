#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt GUI 프로그램에 필요한 헤더 파일들
#include <QMainWindow>
#include <QComboBox>
#include <QTextEdit>
#include <QMap>
#include <QList>
#include <QPair>

// 메인 윈도우 클래스
// 사용자가 출발지와 도착지를 선택하고
// 최단 경로를 확인할 수 있는 GUI를 제공한다.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 생성자
    MainWindow(QWidget *parent = nullptr);

    // 소멸자
    ~MainWindow();

private:
    // 출발지 선택 콤보박스
    QComboBox *startBox;

    // 도착지 선택 콤보박스
    QComboBox *endBox;

    // 최단 경로 결과를 출력하는 텍스트 박스
    QTextEdit *resultBox;

    // 그래프 자료구조
    // Key : 현재 건물 이름
    // Value : 연결된 건물 이름과 이동 거리
    //
    // 예)
    // 공대7호관
    // ├─ 공대8호관 (120m)
    // └─ 학생회관 (150m)
    QMap<QString, QList<QPair<QString, int>>> graph;

    // 캠퍼스 건물 정보를 그래프로 생성하는 함수
    void makeGraph();

    // 다익스트라 알고리즘을 이용하여
    // 최단 경로를 계산하는 함수
    void findPath();
};

#endif // MAINWINDOW_H