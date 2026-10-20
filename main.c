
#include "raylib.h"
#include <stdio.h>

int main(void)
{
    InitWindow(1000, 650, "Supply Chain Simulator");
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
        Rectangle buyButton = {50, 420, 250, 60};
        Rectangle sellButton = {350, 420, 250, 60};
        Rectangle nextButton = {650, 420, 250, 60};

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

        DrawText("SUPPLY CHAIN SIMULATOR",
                 50, 30, 32, DARKBLUE);

        DrawText(TextFormat("DAY %d", day),
                 50, 85, 24, DARKGRAY);

        DrawText(TextFormat("Cash: EUR %d", cash),
                 50, 130, 24, DARKGREEN);

        DrawText(TextFormat("Warehouse stock: %d units", stock),
                 50, 175, 22, DARKGRAY);

        DrawText(TextFormat("Customer demand: %d units", demand),
                 50, 215, 22, DARKGRAY);

        DrawText(TextFormat("Total units sold: %d", totalSold),
                 50, 255, 22, DARKGRAY);

        DrawText(TextFormat("Revenue: EUR %d", revenue),
                 500, 130, 22, DARKGREEN);

        DrawText(TextFormat("Purchases: EUR %d", purchaseCosts),
                 500, 175, 22, MAROON);

        DrawText(TextFormat("Profit: EUR %d", profit),
                 500, 220, 24, profit >= 0 ? DARKGREEN : RED);

        // Supplier delivery status
        if (pendingStock > 0)
        {
            DrawText(
                TextFormat("Delivery: %d units - arrives Day %d",
                           pendingStock, deliveryDay),
                50, 305, 20, ORANGE
            );
        }
        else
        {
            DrawText("Delivery: No orders in transit",
                     50, 305, 20, GRAY);
        }

        // Latest action feedback
        DrawText("Latest update:", 50, 350, 18, DARKBLUE);
        DrawText(status, 190, 350, 18, DARKGRAY);

        // Buttons
        DrawRectangleRec(buyButton, BLUE);
        DrawText("Order 5 Units - EUR 40",
                 65, 440, 19, WHITE);

        DrawRectangleRec(sellButton, DARKGREEN);
        DrawText("Sell to Customers",
                 375, 440, 20, WHITE);

        DrawRectangleRec(nextButton, DARKBLUE);
        DrawText("Advance Day",
                 700, 440, 20, WHITE);

        DrawText("Orders take two days to arrive. Each unit sells for EUR 15.",
                 50, 535, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
