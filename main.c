
#include "raylib.h"
#include <stdio.h>

int main(void)
{
    InitWindow(1000, 750, "Supply Chain Simulator");
    SetTargetFPS(60);

    // Load Roboto font
    Font appFont = LoadFontEx(
        "C:/Users/perit/supplyChainSimulator/Roboto-Regular.ttf",
        32, NULL, 0
    );

    if (!IsFontValid(appFont))
    {
        TraceLog(LOG_ERROR, "Roboto font failed to load!");
        CloseWindow();
        return 1;
    }

    TraceLog(LOG_INFO, "Roboto font loaded successfully!");

    int day = 1;
    int cash = 200;
    int stock = 10;
    int demand = 5;
    int totalSold = 0;

    int revenue = 0;
    int purchaseCosts = 0;
    int operatingCosts = 0;

    int pendingStock = 0;
    int deliveryDay = 0;

    const int dailyOperatingCost = 10;

    char status[128] = "Welcome! Manage your supply chain.";

    while (!WindowShouldClose())
    {
        Rectangle buyButton = {40, 535, 280, 65};
        Rectangle sellButton = {360, 535, 280, 65};
        Rectangle nextButton = {680, 535, 260, 65};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouse = GetMousePosition();

            // Order stock from supplier
            if (CheckCollisionPointRec(mouse, buyButton))
            {
                if (pendingStock > 0)
                {
                    snprintf(status, sizeof(status),
                             "Wait for your current delivery.");
                }
                else if (cash < 40)
                {
                    snprintf(status, sizeof(status),
                             "Not enough cash to order stock!");
                }
                else
                {
                    cash -= 40;
                    purchaseCosts += 40;

                    pendingStock = 5;
                    deliveryDay = day + 2;

                    snprintf(status, sizeof(status),
                             "Order placed! 5 units arrive on Day %d.",
                             deliveryDay);
                }
            }

            // Sell goods to customers
            else if (CheckCollisionPointRec(mouse, sellButton))
            {
                if (demand == 0)
                {
                    snprintf(status, sizeof(status),
                             "Today's demand has already been served.");
                }
                else
                {
                    int sold = demand;

                    if (sold > stock)
                    {
                        sold = stock;
                    }

                    stock -= sold;
                    cash += sold * 15;
                    revenue += sold * 15;
                    totalSold += sold;

                    int unmetDemand = demand - sold;
                    demand = 0;

                    if (unmetDemand > 0)
                    {
                        snprintf(status, sizeof(status),
                                 "Sold %d units. %d customers unserved.",
                                 sold, unmetDemand);
                    }
                    else
                    {
                        snprintf(status, sizeof(status),
                                 "Sold %d units for EUR %d.",
                                 sold, sold * 15);
                    }
                }
            }

            // Advance day and charge operating costs
            else if (CheckCollisionPointRec(mouse, nextButton))
            {
                day++;

                cash -= dailyOperatingCost;
                operatingCosts += dailyOperatingCost;

                demand = GetRandomValue(3, 9);

                snprintf(status, sizeof(status),
                         "Day %d started. EUR %d operating cost charged.",
                         day, dailyOperatingCost);

                // Receive supplier delivery when due
                if (pendingStock > 0 && day >= deliveryDay)
                {
                    int received = pendingStock;
                    stock += received;

                    pendingStock = 0;
                    deliveryDay = 0;

                    snprintf(status, sizeof(status),
                             "Delivery received! %d units added to stock.",
                             received);
                }
            }
        }

        int profit = revenue - purchaseCosts - operatingCosts;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Header
        DrawTextEx(appFont, "SUPPLY CHAIN SIMULATOR",
                   (Vector2){40, 25}, 30, 1, DARKBLUE);

        DrawTextEx(appFont, TextFormat("DAY %d", day),
                   (Vector2){40, 70}, 22, 1, DARKGRAY);

        // Business overview
        DrawRectangleRounded(
            (Rectangle){40, 110, 440, 145},
            0.08f, 8, (Color){240, 244, 248, 255});

        DrawTextEx(appFont, "BUSINESS OVERVIEW",
                   (Vector2){60, 125}, 20, 1, DARKBLUE);

        DrawTextEx(appFont, TextFormat("Cash: EUR %d", cash),
                   (Vector2){60, 160}, 20, 1, DARKGREEN);

        DrawTextEx(appFont, TextFormat("Warehouse stock: %d units", stock),
                   (Vector2){60, 190}, 18, 1, DARKGRAY);

        DrawTextEx(appFont, TextFormat("Customer demand: %d units", demand),
                   (Vector2){60, 220}, 18, 1, DARKGRAY);

        // Financial summary
        DrawRectangleRounded(
            (Rectangle){500, 110, 440, 145},
            0.08f, 8, (Color){240, 244, 248, 255});

        DrawTextEx(appFont, "FINANCIAL SUMMARY",
                   (Vector2){520, 125}, 20, 1, DARKBLUE);

        DrawTextEx(appFont, TextFormat("Revenue: EUR %d", revenue),
                   (Vector2){520, 158}, 19, 1, DARKGREEN);

        DrawTextEx(appFont, TextFormat("Purchases: EUR %d", purchaseCosts),
                   (Vector2){520, 184}, 19, 1, MAROON);

        DrawTextEx(appFont, TextFormat("Operating costs: EUR %d", operatingCosts),
                   (Vector2){520, 210}, 17, 1, MAROON);

        DrawTextEx(appFont, TextFormat("Profit: EUR %d", profit),
                   (Vector2){760, 232}, 19, 1,
                   profit >= 0 ? DARKGREEN : RED);

        DrawTextEx(appFont, TextFormat("Total units sold: %d", totalSold),
                   (Vector2){40, 270}, 18, 1, DARKGRAY);

        // Supply chain flow
        DrawTextEx(appFont, "SUPPLY CHAIN FLOW",
                   (Vector2){40, 300}, 22, 1, DARKBLUE);

        // Supplier
        DrawRectangleRounded(
            (Rectangle){40, 345, 250, 105},
            0.12f, 8, (Color){230, 230, 230, 255});

        DrawTextEx(appFont, "SUPPLIER",
                   (Vector2){105, 360}, 20, 1, DARKBLUE);

        if (pendingStock > 0)
        {
            DrawTextEx(appFont, TextFormat("%d units in transit", pendingStock),
                       (Vector2){60, 395}, 17, 1, ORANGE);

            DrawTextEx(appFont, TextFormat("Arrives Day %d", deliveryDay),
                       (Vector2){60, 420}, 17, 1, DARKGRAY);
        }
        else
        {
            DrawTextEx(appFont, "No delivery pending",
                       (Vector2){60, 395}, 17, 1, DARKGRAY);
        }

        // Supplier to warehouse
        DrawLineEx(
            (Vector2){290, 395},
            (Vector2){345, 395},
            4, pendingStock > 0 ? ORANGE : GRAY);

        DrawTriangle(
            (Vector2){345, 387},
            (Vector2){345, 403},
            (Vector2){357, 395},
            pendingStock > 0 ? ORANGE : GRAY);

        // Warehouse
        DrawRectangleRounded(
            (Rectangle){360, 345, 250, 105},
            0.12f, 8, SKYBLUE);

        DrawTextEx(appFont, "WAREHOUSE",
                   (Vector2){415, 360}, 20, 1, DARKBLUE);

        DrawTextEx(appFont, TextFormat("%d units in stock", stock),
                   (Vector2){390, 400}, 19, 1, DARKBLUE);

        // Warehouse to customers
        DrawLineEx(
            (Vector2){610, 395},
            (Vector2){665, 395},
            4, stock > 0 ? DARKGREEN : GRAY);

        DrawTriangle(
            (Vector2){665, 387},
            (Vector2){665, 403},
            (Vector2){677, 395},
            stock > 0 ? DARKGREEN : GRAY);

        // Customers
        DrawRectangleRounded(
            (Rectangle){680, 345, 260, 105},
            0.12f, 8, (Color){210, 245, 220, 255});

        DrawTextEx(appFont, "CUSTOMERS",
                   (Vector2){750, 360}, 20, 1, DARKGREEN);

        DrawTextEx(appFont, TextFormat("Demand: %d units", demand),
                   (Vector2){735, 400}, 19, 1, DARKGREEN);

        // Latest update
        DrawRectangleRounded(
            (Rectangle){40, 465, 900, 45},
            0.08f, 8, (Color){245, 245, 245, 255});

        DrawTextEx(appFont, "LATEST UPDATE",
                   (Vector2){55, 478}, 17, 1, DARKBLUE);

        DrawTextEx(appFont, status,
                   (Vector2){205, 478}, 17, 1, DARKGRAY);

        // Action buttons
        DrawRectangleRounded(
            (Rectangle){40, 535, 280, 65},
            0.12f, 8, BLUE);

        DrawTextEx(appFont, "Order 5 Units - EUR 40",
                   (Vector2){55, 557}, 18, 1, WHITE);

        DrawRectangleRounded(
            (Rectangle){360, 535, 280, 65},
            0.12f, 8, DARKGREEN);

        DrawTextEx(appFont, "Sell to Customers",
                   (Vector2){390, 557}, 20, 1, WHITE);

        DrawRectangleRounded(
            (Rectangle){680, 535, 260, 65},
            0.12f, 8, DARKBLUE);

        DrawTextEx(appFont, "Advance Day",
                   (Vector2){750, 557}, 20, 1, WHITE);

        DrawTextEx(appFont, "Supplier orders take two days to arrive.",
                   (Vector2){40, 625}, 16, 1, GRAY);

        EndDrawing();
    }

    UnloadFont(appFont);
    CloseWindow();

    return 0;
}
