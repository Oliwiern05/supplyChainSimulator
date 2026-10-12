
#include "raylib.h"

int main(void)
{
    InitWindow(1000, 650, "Supply Chain Simulator");
    SetTargetFPS(60);

    int day = 1;
    int cash = 200;
    int stock = 10;
    int demand = 5;
    int totalSold = 0;
    int profit = 0;

    int pendingStock = 0;
    int deliveryDay = 0;

    while (!WindowShouldClose())
    {
        Rectangle buyButton = {50, 400, 250, 60};
        Rectangle sellButton = {350, 400, 250, 60};
        Rectangle nextButton = {650, 400, 250, 60};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouse = GetMousePosition();

            // Order stock from the supplier
            if (CheckCollisionPointRec(mouse, buyButton))
            {
                if (cash >= 40 && pendingStock == 0)
                {
                    cash -= 40;
                    profit -= 40;

                    pendingStock = 5;
                    deliveryDay = day + 2;
                }
            }

            // Sell goods to customers
            if (CheckCollisionPointRec(mouse, sellButton))
            {
                int sold = demand;

                if (sold > stock)
                    sold = stock;

                stock -= sold;
                cash += sold * 15;
                profit += sold * 15;
                totalSold += sold;

                demand = 0;
            }

            // Advance to the next day
            if (CheckCollisionPointRec(mouse, nextButton))
            {
                day++;

                demand = GetRandomValue(3, 9);

                // Receive the supplier delivery
                if (pendingStock > 0 && day >= deliveryDay)
                {
                    stock += pendingStock;
                    pendingStock = 0;
                    deliveryDay = 0;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("SUPPLY CHAIN SIMULATOR",
                 50, 35, 32, DARKBLUE);

        DrawText(TextFormat("DAY %d", day),
                 50, 100, 24, DARKGRAY);

        DrawText(TextFormat("Cash: EUR %d", cash),
                 50, 150, 24, DARKGREEN);

        DrawText(TextFormat("Warehouse stock: %d units", stock),
                 50, 200, 22, DARKGRAY);

        DrawText(TextFormat("Customer demand: %d units", demand),
                 50, 245, 22, DARKGRAY);

        DrawText(TextFormat("Total units sold: %d", totalSold),
                 50, 290, 22, DARKGRAY);

        DrawText(TextFormat("Profit: EUR %d", profit),
                 500, 150, 24, DARKGREEN);

        // Supplier delivery status
        if (pendingStock > 0)
        {
            DrawText(
                TextFormat("Delivery: %d units - arrives Day %d",
                           pendingStock, deliveryDay),
                50, 340, 20, ORANGE
            );
        }
        else
        {
            DrawText("Delivery: No orders in transit",
                     50, 340, 20, GRAY);
        }

        DrawRectangleRec(buyButton, BLUE);
        DrawText("Order 5 Units - EUR 40",
                 65, 420, 19, WHITE);

        DrawRectangleRec(sellButton, DARKGREEN);
        DrawText("Sell to Customers",
                 375, 420, 20, WHITE);

        DrawRectangleRec(nextButton, DARKBLUE);
        DrawText("Advance Day",
                 700, 420, 20, WHITE);

        DrawText("Orders take two days to arrive.",
                 50, 530, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
