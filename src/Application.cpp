#include "Application.hpp"

#include <optional>


// ============================================================
// CONSTRUCTOR
// ============================================================

Application::Application()
    : window(
          sf::VideoMode({
              WINDOW_WIDTH,
              WINDOW_HEIGHT
          }),
          "A* PathFinding"
      ),

      font(
          "assets/Roboto-Regular.ttf"
      ),

      titleText(
          font,
          "A* PATHFINDING VISUALIZER",
          20
      ),

      controlsText(
          font,
          "LMB Start  |  RMB Goal  |  MMB Walls  |  SPACE Run  |  C Clear  |  R Reset",
          13
      ),

      statusText(
          font,
          "STATUS: READY",
          14
      ),

      uiBar(
          sf::Vector2f(
              static_cast<float>(
                  WINDOW_WIDTH
              ),

              static_cast<float>(
                  UI_HEIGHT
              )
          )
      ),

      separator(
          sf::Vector2f(
              static_cast<float>(
                  WINDOW_WIDTH
              ),
              2.f
          )
      ),

      statusBox(
          sf::Vector2f(
              165.f,
              34.f
          )
      ),

      grid(UI_HEIGHT)
{
    // ========================================================
    // TITULO
    // ========================================================

    titleText.setPosition({
        16.f,
        12.f
    });


    titleText.setFillColor(
        sf::Color(
            235,
            238,
            245
        )
    );


    // ========================================================
    // CONTROLES
    // ========================================================

    controlsText.setPosition({
        16.f,
        57.f
    });


    controlsText.setFillColor(
        sf::Color(
            165,
            170,
            185
        )
    );


    // ========================================================
    // BARRA SUPERIOR
    // ========================================================

    uiBar.setPosition({
        0.f,
        0.f
    });


    uiBar.setFillColor(
        sf::Color(
            20,
            20,
            20
        )
    );


    // ========================================================
    // SEPARADOR
    // ========================================================

    separator.setPosition({
        0.f,
        static_cast<float>(
            UI_HEIGHT - 2
        )
    });


    separator.setFillColor(
        sf::Color(
            55,
            60,
            72
        )
    );


    // ========================================================
    // STATUS
    // ========================================================

    statusBox.setPosition({
        620.f,
        11.f
    });


    statusBox.setFillColor(
        sf::Color(
            35,
            39,
            48
        )
    );


    statusText.setPosition({
        637.f,
        19.f
    });


    statusText.setFillColor(
        sf::Color(
            200,
            205,
            215
        )
    );
}


// ============================================================
// LOOP PRINCIPAL
// ============================================================

void Application::run()
{
    while (window.isOpen())
    {
        processEvents();

        update();

        render();
    }
}


// ============================================================
// EVENTOS
// ============================================================

void Application::processEvents()
{
    while (
        const std::optional event =
            window.pollEvent()
    )
    {
        if (
            event->is<
                sf::Event::Closed
            >()
        )
        {
            window.close();
        }


        if (
            const auto* keyEvent =
                event->getIf<
                    sf::Event::KeyPressed
                >()
        )
        {
            handleKeyboard(
                *keyEvent
            );
        }


        if (
            const auto* mouseEvent =
                event->getIf<
                    sf::Event::MouseButtonPressed
                >()
        )
        {
            handleMousePressed(
                *mouseEvent
            );
        }


        if (
            const auto* mouseEvent =
                event->getIf<
                    sf::Event::MouseButtonReleased
                >()
        )
        {
            handleMouseReleased(
                *mouseEvent
            );
        }


        if (
            const auto* moveEvent =
                event->getIf<
                    sf::Event::MouseMoved
                >()
        )
        {
            handleMouseMoved(
                *moveEvent
            );
        }
    }
}


// ============================================================
// TECLADO
// ============================================================

void Application::handleKeyboard(
    const sf::Event::KeyPressed& event
)
{
    // ========================================================
    // RESET COMPLETO
    // ========================================================

    if (
        event.code ==
        sf::Keyboard::Key::R
    )
    {
        astar.reset();

        grid.reset();

        return;
    }


    // ========================================================
    // LIMPIAR RESULTADO
    // ========================================================

    if (
        event.code ==
        sf::Keyboard::Key::C
    )
    {
        astar.reset();

        grid.clearSearch();

        return;
    }


    // ========================================================
    // EJECUTAR A*
    // ========================================================

    if (
        event.code ==
        sf::Keyboard::Key::Space
    )
    {
        if (
            grid.hasStart() &&
            grid.hasGoal() &&
            !astar.isRunning()
        )
        {
            astar.begin(
                grid.cells(),

                grid.getStartPosition(),

                grid.getGoalPosition()
            );


            stepClock.restart();
        }
    }
}


// ============================================================
// MOUSE PRESIONADO
// ============================================================

void Application::handleMousePressed(
    const sf::Event::MouseButtonPressed& event
)
{
    if (astar.isRunning())
    {
        return;
    }


    grid.handleMousePressed(
        event
    );
}


// ============================================================
// MOUSE LIBERADO
// ============================================================

void Application::handleMouseReleased(
    const sf::Event::MouseButtonReleased& event
)
{
    grid.handleMouseReleased(
        event
    );
}


// ============================================================
// MOUSE EN MOVIMIENTO
// ============================================================

void Application::handleMouseMoved(
    const sf::Event::MouseMoved& event
)
{
    if (astar.isRunning())
    {
        return;
    }


    grid.handleMouseMoved(
        event
    );
}


// ============================================================
// UPDATE GENERAL
// ============================================================

void Application::update()
{
    updateAStar();

    updateStatus();
}


// ============================================================
// ACTUALIZAR A*
// ============================================================

void Application::updateAStar()
{
    if (!astar.isRunning())
    {
        return;
    }


    if (
        stepClock
            .getElapsedTime()
            .asMilliseconds() >= 40
    )
    {
        astar.step(
            grid.cells()
        );


        stepClock.restart();
    }
}


// ============================================================
// ACTUALIZAR STATUS
// ============================================================

void Application::updateStatus()
{
    // ========================================================
    // SEARCHING
    // ========================================================

    if (astar.isRunning())
    {
        statusText.setString(
            "STATUS: SEARCHING..."
        );


        statusText.setFillColor(
            sf::Color(
                66,
                165,
                245
            )
        );


        statusBox.setFillColor(
            sf::Color(
                25,
                55,
                85
            )
        );


        return;
    }


    // ========================================================
    // PATH FOUND
    // ========================================================

    if (astar.pathFound())
    {
        statusText.setString(
            "STATUS: PATH FOUND"
        );


        statusText.setFillColor(
            sf::Color(
                72,
                220,
                140
            )
        );


        statusBox.setFillColor(
            sf::Color(
                25,
                70,
                52
            )
        );


        return;
    }


    // ========================================================
    // NO SOLUTION
    // ========================================================

    if (astar.hasFinished())
    {
        statusText.setString(
            "STATUS: NO SOLUTION"
        );


        statusText.setFillColor(
            sf::Color(
                255,
                100,
                100
            )
        );


        statusBox.setFillColor(
            sf::Color(
                80,
                35,
                40
            )
        );


        return;
    }


    // ========================================================
    // READY
    // ========================================================

    statusText.setString(
        "STATUS: READY"
    );


    statusText.setFillColor(
        sf::Color(
            200,
            205,
            215
        )
    );


    statusBox.setFillColor(
        sf::Color(
            35,
            39,
            48
        )
    );
}


// ============================================================
// RENDER
// ============================================================

void Application::render()
{
    window.clear(
        sf::Color(
            18,
            20,
            25
        )
    );


    grid.draw(
        window
    );


    drawInterface();


    window.display();
}


// ============================================================
// INTERFAZ
// ============================================================

void Application::drawInterface()
{
    window.draw(
        uiBar
    );


    window.draw(
        separator
    );


    window.draw(
        titleText
    );


    window.draw(
        controlsText
    );


    window.draw(
        statusBox
    );


    window.draw(
        statusText
    );
}