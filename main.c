
#include "raylib.h"
#include <stdio.h>

int main(void)
{
    InitWindow(1000, 750, "Supply Chain Simulator");
    SetTargetFPS(60);

    int day = 1;
    int cash = 200;
    int stock = 10;
    int demand = 5;
    int totalSold = 0;

    // Financial tracking
    int revenue = 0;
    int purchaseCosts = 0;

    // Supplier delivery tracking
    int pendingStock = 0;
    int deliveryDay = 0;

    char status[128] = "Welcome! Manage your supply chain.";

    while (!WindowShouldClose())
    {
        Rectangle buyButton = {40, 525, 280, 65};
        Rectangle sellButton = {360, 525, 280, 65};
        Rectangle nextButton = {680, 525, 260, 65};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouse = GetMousePosition();

            // Order 5 units from the supplier
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
            if (CheckCollisionPointRec(mouse, sellButton))
            {
                if (demand == 0)
                {
                    snprintf(status, sizeof(status),
                             "You have already served today's demand.");
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
                                 "Sold %d units. %d customers went unserved.",
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

            // Advance to the next day
            if (CheckCollisionPointRec(mouse, nextButton))
            {
                day++;

                demand = GetRandomValue(3, 9);

                // Receive supplier delivery when it is due
                if (pendingStock > 0 && day >= deliveryDay)
                {
                    stock += pendingStock;

                    snprintf(status, sizeof(status),
                             "Delivery received! %d units added to stock.",
                             pendingStock);

                    pendingStock = 0;
                    deliveryDay = 0;
                }
                else
                {
                    snprintf(status, sizeof(status),
                             "Day %d started. New demand: %d units.",
                             day, demand);
                }
            }
        }

        // Profit is revenue minus purchasing costs
        int profit = revenue - purchaseCosts;


        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Header
        DrawText("SUPPLY CHAIN SIMULATOR", 40, 25, 30, DARKBLUE);
        DrawText(TextFormat("DAY %d", day), 40, 70, 22, DARKGRAY);

        // Dashboard cards
        DrawRectangleRounded((Rectangle){40, 110, 440, 145},
                             0.08f, 8, (Color){240, 244, 248, 255});
        DrawText("BUSINESS OVERVIEW", 60, 125, 20, DARKBLUE);
        DrawText(TextFormat("Cash: EUR %d", cash), 60, 160, 20, DARKGREEN);
        DrawText(TextFormat("Warehouse stock: %d", stock), 60, 190, 20, DARKGRAY);
        DrawText(TextFormat("Customer demand: %d", demand), 60, 220, 20, DARKGRAY);

        DrawRectangleRounded((Rectangle){500, 110, 440, 145},
                             0.08f, 8, (Color){240, 244, 248, 255});
        DrawText("FINANCIAL SUMMARY", 520, 125, 20, DARKBLUE);
        DrawText(TextFormat("Revenue: EUR %d", revenue), 520, 160, 20, DARKGREEN);
        DrawText(TextFormat("Purchases: EUR %d", purchaseCosts), 520, 190, 20, MAROON);
        DrawText(TextFormat("Profit: EUR %d", profit), 520, 220, 20,
                 profit >= 0 ? DARKGREEN : RED);

        // Supply chain
        DrawText("SUPPLY CHAIN FLOW", 40, 285, 22, DARKBLUE);

        // Supplier
        DrawRectangleRounded((Rectangle){40, 330, 250, 105},
                             0.12f, 8, (Color){230, 230, 230, 255});
        DrawText("SUPPLIER", 105, 345, 20, DARKBLUE);

        if (pendingStock > 0)
        {
            DrawText(TextFormat("%d units in transit", pendingStock),
                     60, 380, 17, ORANGE);
            DrawText(TextFormat("Arrives Day %d", deliveryDay),
                     60, 405, 17, DARKGRAY);
        }
        else
        {
            DrawText("No delivery pending", 60, 385, 17, DARKGRAY);
        }

        // Arrow to warehouse
        DrawLineEx((Vector2){290, 380}, (Vector2){345, 380}, 4, GRAY);
        DrawTriangle((Vector2){345, 372}, (Vector2){345, 388},
                     (Vector2){357, 380}, GRAY);

        // Warehouse
        DrawRectangleRounded((Rectangle){360, 330, 250, 105},
                             0.12f, 8, SKYBLUE);
        DrawText("WAREHOUSE", 415, 345, 20, DARKBLUE);
        DrawText(TextFormat("%d units in stock", stock),
                 390, 385, 19, DARKBLUE);

        // Arrow to customers
        DrawLineEx((Vector2){610, 380}, (Vector2){665, 380},
                   4, stock > 0 ? DARKGREEN : GRAY);
        DrawTriangle((Vector2){665, 372}, (Vector2){665, 388},
                     (Vector2){677, 380},
                     stock > 0 ? DARKGREEN : GRAY);

        // Customers
        DrawRectangleRounded((Rectangle){680, 330, 260, 105},
                             0.12f, 8, (Color){210, 245, 220, 255});
        DrawText("CUSTOMERS", 750, 345, 20, DARKGREEN);
        DrawText(TextFormat("Demand: %d units", demand),
                 735, 385, 19, DARKGREEN);

        // Latest update in its own area
        DrawRectangleRounded((Rectangle){40, 455, 900, 45},
                             0.08f, 8, (Color){245, 245, 245, 255});
        DrawText("LATEST UPDATE", 55, 468, 17, DARKBLUE);
        DrawText(status, 205, 468, 17, DARKGRAY);

        // Action buttons
        DrawRectangleRounded((Rectangle){40, 525, 280, 65},
                             0.12f, 8, BLUE);
        DrawText("Order 5 Units - EUR 40", 55, 547, 18, WHITE);

        DrawRectangleRounded((Rectangle){360, 525, 280, 65},
                             0.12f, 8, DARKGREEN);
        DrawText("Sell to Customers", 390, 547, 20, WHITE);

        DrawRectangleRounded((Rectangle){680, 525, 260, 65},
                             0.12f, 8, DARKBLUE);
        DrawText("Advance Day", 750, 547, 20, WHITE);

        DrawText("Supplier orders take two days to arrive.",
                 40, 620, 16, GRAY);

        EndDrawing();

    }

    CloseWindow();
    return 0;
}
