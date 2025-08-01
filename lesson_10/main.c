// #include "10.1/digital.c"
// #include <stdint.h>
// #include <avr/io.h>

// const uint8_t PIN_FOR_QUARTERS = 2; 
// const uint8_t PIN_TO_SWITCH_ITEM = 3;

// enum {
//     AWAIT,
//     DISPENSE,
//     RETURN_QUARTERS
// };

// struct items {
//     uint8_t cost_in_quarters;
//     uint8_t stock;
//     char name[10];
// };

// struct state_variables {
//     uint8_t quarter_count;
//     uint8_t current_state;
//     struct items avaliable_options[3];
//     struct items current_item;
// };

// void await(struct state_variables* state) { 
//     bool quarter;
//     bool switch_item;

//     digital_read(PIN_FOR_QUARTERS, &quarter);
//     digital_read(PIN_TO_SWITCH_ITEM, &switch_item);
    
//     while (state->quarter_count > state->current_item.cost_in_quarters) {
//         if (quarter) {
//             state->quarter_count++;
//         }
//         if (switch_item) {
//             state->current_item = state->avaliable_options[(state->current_item.cost_in_quarters + 1) % 3];
//         }
//     }
//     state->current_state = DISPENSE;
// }

// void dispense(struct state_variables* state) {
//     if (state->quarter_count >= state->current_item.cost_in_quarters && state->current_item.stock > 0) {
//         state->quarter_count -= state->current_item.cost_in_quarters;
//         state->current_item.stock--;
//     } else {
//         // Not enough quarters or out of stock
//         state->current_state = RETURN_QUARTERS;
//     }
// }

// void return_quarters(struct state_variables* state) {
//     // Return the quarters to the user
//     state->quarter_count = 0;
//     state->current_state = AWAIT; // Go back to the await state
// }

// void (*STATE_MACHINE_ARRAY[])(struct state_variables* state) = {&await, &dispense, &return_quarters};

// void state_machine(void) {

//     static struct state_variables machine_state = {
//         .quarter_count = 0,
//         .avaliable_options = {
//             {10, 10, "DOG"},
//             {5, 5, "CAT"},
//             {8, 8, "FISH"}
//         }
//     };

//     machine_state.current_item = machine_state.avaliable_options[0];

//     STATE_MACHINE_ARRAY[machine_state.current_state](&machine_state);
// }

// int main(void) {
//     digital_pin_mode(PIN_FOR_QUARTERS, INPUT);
//     digital_pin_mode(PIN_TO_SWITCH_ITEM, INPUT);
//     while (1) {
//         state_machine();
//     }
//     return 0;
// }

