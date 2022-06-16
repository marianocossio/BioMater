#include "graph.h"

Graph::Graph(QWidget *parent) : QOpenGLWidget(parent)
{
    // setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    colors.push_back(QColor(52, 168, 83));
    colors.push_back(QColor(66, 133, 244));
    colors.push_back(QColor(237, 28, 36));
    colors.push_back(QColor(251, 188, 5));
    colors.push_back(QColor(165, 160, 169));
    colors.push_back(QColor(236, 159, 159));
    colors.push_back(QColor(26, 152, 117));
    colors.push_back(QColor(140, 161, 63));

    numberOfActiveChannels = 0;
    graphicZoom = 1;
    viewChannelNames = true;
}

Graph::~Graph()
{

}

void Graph::addData(DataSet data)
{
    if (localSignals.size() >= DRAWN_SAMPLES)
        localSignals.clear();

    localSignals.push_back(data);

    update();
}

void Graph::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
}

void Graph::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0f, DRAWN_SAMPLES * 1.0f, 0.0f, DataSet::DataSet_8 * SIGNAL_EXCURSION, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void Graph::paintGL()
{
    if (localSignals.size() > 1)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        float signalOffset = SIGNAL_EXCURSION / 2.0f;
        int currentChannel = 0;

        glClear(GL_DEPTH_BUFFER_BIT);

        glPushMatrix();

        float scaleRatio = localSignals[localSignals.size() - 1].getDataType() / (localSignals[localSignals.size() - 1].numberOfActiveChannels() * 1.0f);

        glScalef(1.0f, scaleRatio, 1.0f);

        glBegin(GL_LINES);

        for (int channelIndex = 0; channelIndex < localSignals[localSignals.size() - 1].getDataType(); channelIndex++)
            if (localSignals[localSignals.size() - 1].channelIsActive(channelIndex))
            {
                glColor3f(colors[channelIndex].redF() * 0.15f, colors[channelIndex].greenF() * 0.15f, colors[channelIndex].blueF() * 0.15f);

                glVertex3f(0.0f,
                           signalOffset + (signalOffset * 2.0f) * (currentChannel * 1.0f),
                           0.0f);
                glVertex3f(DRAWN_SAMPLES * 1.0f,
                           signalOffset + (signalOffset * 2.0f) * (currentChannel * 1.0f),
                           0.0f);

                currentChannel++;
            }

        glEnd();

        currentChannel = 0;

        glBegin(GL_LINES);

        for (int channelIndex = 0; channelIndex < localSignals[localSignals.size() - 1].getDataType(); channelIndex++)
            if (localSignals[localSignals.size() - 1].channelIsActive(channelIndex))
            {
                for (int signalIndex = 0; signalIndex < (localSignals.size() - 1); signalIndex++)
                {
                    glColor3f(colors[channelIndex].redF(), colors[channelIndex].greenF(), colors[channelIndex].blueF());

                    glVertex3f(signalIndex * 1.0f,
                               localSignals[signalIndex].channelData(channelIndex) * (4.5 / SIGNAL_MAX_VALUE) * (float) graphicZoom + signalOffset + (signalOffset * 2.0f) * (currentChannel * 1.0f),
                               0.0f);
                    glVertex3f((signalIndex + 1) * 1.0f,
                               localSignals[signalIndex + 1].channelData(channelIndex) * (4.5 / SIGNAL_MAX_VALUE) * (float) graphicZoom + signalOffset + (signalOffset * 2.0f) * (currentChannel * 1.0f),
                               0.0f);
                    }

                currentChannel++;
            }

        glEnd();

        glPopMatrix();

        if (viewChannelNames)
            drawChannelNumbers();

        glFlush();
    }
}

void Graph::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case 'c':
        viewChannelNames = !viewChannelNames;
        break;

    case 'C':
        viewChannelNames = !viewChannelNames;
        break;

    case '1':
        graphicZoom = 1;
        break;

    case Qt::Key_Escape:
        if (isFullScreen())
        {
            showMaximized();

            setCursor(Qt::ArrowCursor);
        }
        break;

    case Qt::Key_F11:
        if (isFullScreen())
        {
            showMaximized();

            setCursor(Qt::ArrowCursor);
        }

        else
        {
            showFullScreen();

            setCursor(Qt::BlankCursor);
        }
        break;

    default:
        break;
    }
}

void Graph::wheelEvent(QWheelEvent *event)
{
    if (!event->angleDelta().isNull())
    {
        if (event->angleDelta().y() > 0)
            graphicZoom++;
        else
            if (--graphicZoom < 1)
                graphicZoom = 1;

        update();
    }
}

void Graph::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (isFullScreen())
    {
        showMaximized();

        setCursor(Qt::ArrowCursor);
    }

    else
    {
        showFullScreen();

        setCursor(Qt::BlankCursor);
    }
}

void Graph::drawChannelNumbers()
{
    QPainter textPainter(this);
    QFont font = textPainter.font();
    font.setPointSize(height() / 60);

    textPainter.setFont(font);

    int currentChannel = 0;

    for (int channelIndex = 0; channelIndex < localSignals[localSignals.size() - 1].getDataType(); channelIndex++)
        if (localSignals[localSignals.size() - 1].channelIsActive(channelIndex))
        {
            textPainter.setPen(colors[channelIndex]);

            textPainter.drawText(width() / 30,
                                (height() - (currentChannel + 1) * (height() / localSignals[localSignals.size() - 1].numberOfActiveChannels())) + (2 * font.pointSize()),
                                QString::fromStdString(localSignals[localSignals.size() - 1].channelName(channelIndex)));

            textPainter.drawText(width() - 3 * font.pointSize(),
                                (height() - (currentChannel + 1) * (height() / localSignals[localSignals.size() - 1].numberOfActiveChannels())) + (2 * font.pointSize()),
                                "x" + QString::number(localSignals[localSignals.size() - 1].channelGain(channelIndex)));

            currentChannel++;
        }

    textPainter.setPen(QColor(255, 255, 255));

    textPainter.drawText(width() - 16 * font.pointSize(),
                         1.5 * font.pointSize(),
                         "Graphic Zoom: x" + QString::number(graphicZoom));

    textPainter.end();
}
