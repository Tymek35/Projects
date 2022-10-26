import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-gameboard',
  templateUrl: './gameboard.component.html',
  styleUrls: ['./gameboard.component.scss']
})
export class GameboardComponent implements OnInit {
    fields: string[] = new Array(81);
    choosen_field: number = -1;
    possible_colors: string[] = ['blue', 'purple', 'green', 'yellow', 'orange', 'brown', 'light_blue'];
    game_over: boolean = false;
    to_clear: number[] = new Array();
    score: number = 0;

    constructor() { }

    ngOnInit(): void {
        for (let i = 0; i < 81; i++) {
            this.fields[i] = 'none';
        }
        
        this.add_random();
    }

    choose_field(field_num: number): void {
        if(this.fields[field_num] != 'none') {
            this.choosen_field = field_num;
        } else {
            if(this.choosen_field != -1 && this.check_if_move_possible(field_num)) {
                this.fields[field_num] = this.fields[this.choosen_field];
                this.fields[this.choosen_field] = 'none';
                this.choosen_field = -1;
                this.check_horizontally();
                this.check_vertically();
                this.check_diagonally();
                if(this.to_clear.length == 0) {
                    this.add_random();
                    this.check_horizontally();
                    this.check_vertically();
                    this.check_diagonally();
                }
                this.clear_rows();
            }
        }
    }

    check_if_move_possible(field_num: number): boolean {
        let stack: number[] = new Array();
        stack.push(this.choosen_field);
        let fields_visited: boolean[] = new Array(81);
        for(let i = 0; i < 81; i++) {
            fields_visited[i] = false;
        }
        let reached: boolean = false;

        while(!reached && stack.length != 0) {
            let field: any = stack.pop();
            if(field == field_num) {
                reached = true;
            } else {
                fields_visited[field] = true;
                if(field - 9 > 0 && this.fields[field - 9] == 'none' && !fields_visited[field - 9]) {
                    stack.push(field - 9);
                }
                if(field - 1 > 0 && field % 9 != 0 && this.fields[field - 1] == 'none' && !fields_visited[field - 1]) {
                    stack.push(field - 1);
                }
                if(field + 1 < 81 && field % 9 != 8 && this.fields[field + 1] == 'none' && !fields_visited[field + 1]) {
                    stack.push(field + 1);
                }
                if(field + 9 < 81 && this.fields[field + 9] == 'none' && !fields_visited[field + 9]) {
                    stack.push(field + 9);
                }
            }
        }

        return reached;
    }

    check_horizontally(): void {
        for(let i = 0; i < 9; i++) {
            let current_color: string = 'none';
            let last_color: string = 'none'
            let number_in_color: number = 1;
            for(let j = 0; j < 9; j++) {
                current_color = this.fields[i * 9 + j];
                if(current_color != 'none' && current_color == last_color) {
                    number_in_color++;
                } else {
                    number_in_color = 1;
                }
                
                if(number_in_color >= 5) {
                    this.add_to_clear_horizontally(current_color, j, i);
                    j = 9;
                }

                last_color = current_color;
            }
        }
    }

    add_to_clear_horizontally(color_to_clear: string, j: number, i: number): void {
        let in_row: number = 0;
        for(let c = j; c < 9 && this.fields[i * 9 + c] == color_to_clear; c++) {
            this.to_clear.push(i * 9 + c);
            in_row++;
        }
        
        for(let c = j - 1; c >= 0 && this.fields[i * 9 + c] == color_to_clear; c--) {
            this.to_clear.push(i * 9 + c);
            in_row++;
        }
        
        this.score = this.score + 10 * Math.pow(2, in_row - 5);
    }

    check_vertically(): void {
        for(let i = 0; i < 9; i++) {
            let current_color: string = 'none';
            let last_color: string = 'none'
            let number_in_color: number = 1;
            for(let j = 0; j < 9; j++) {
                current_color = this.fields[i + j * 9];
                if(current_color != 'none' && current_color == last_color) {
                    number_in_color++;
                } else {
                    number_in_color = 1;
                }
                
                if(number_in_color >= 5) {
                    this.add_to_clear_vertically(current_color, j, i);
                    j = 9;
                }

                last_color = current_color;
            }
        }
    }

    add_to_clear_vertically(color_to_clear: string, j: number, i: number): void {
        let in_row: number = 0;
        for(let c = j; c < 9 && this.fields[i + c * 9] == color_to_clear; c++) {
            this.to_clear.push(i + c * 9);
            in_row++;
        }
        
        for(let c = j - 1; c >= 0 && this.fields[i + c * 9] == color_to_clear; c--) {
            this.to_clear.push(i + c * 9);
            in_row++;
        }
        
        this.score = this.score + 10 * Math.pow(2, in_row - 5);
    }

    check_diagonally(): void {
        this.check_diagonally_1();
        this.check_diagonally_2();
        this.check_diagonally_3();
        this.check_diagonally_4();
    }

    check_diagonally_1(): void {
        for(let i = 0; i < 5; i++) {
            let current_color: string = 'none';
            let last_color: string = 'none'
            let number_in_color: number = 1;
            for(let j = 0; j < 9 - i; j++) {
                current_color = this.fields[i + j * 10];
                if(current_color != 'none' && current_color == last_color) {
                    number_in_color++;
                } else {
                    number_in_color = 1;
                }
                
                if(number_in_color >= 5) {
                    this.add_to_clear_diagonally_1(current_color, j, i);
                    j = 9;
                }

                last_color = current_color;
            }
        }
    }

    add_to_clear_diagonally_1(color_to_clear: string, j: number, i: number): void {
        let in_row: number = 0;
        for(let c = j; c < 9 - i && this.fields[i + c * 10] == color_to_clear; c++) {
            this.to_clear.push(i + c * 10);
            in_row++;
        }
        
        for(let c = j - 1; c >= 0 && this.fields[i + c * 10] == color_to_clear; c--) {
            this.to_clear.push(i + c * 10);
            in_row++;
        }
        
        this.score = this.score + 10 * Math.pow(2, in_row - 5);
    }

    check_diagonally_2(): void {
        for(let i = 9; i <= 36; i = i + 9) {
            let current_color: string = 'none';
            let last_color: string = 'none'
            let number_in_color: number = 1;
            for(let j = 0; j < 9 - i / 9; j++) {
                current_color = this.fields[i + j * 10];
                if(current_color != 'none' && current_color == last_color) {
                    number_in_color++;
                } else {
                    number_in_color = 1;
                }
                
                if(number_in_color >= 5) {
                    this.add_to_clear_diagonally_2(current_color, j, i);
                    j = 9;
                }

                last_color = current_color;
            }
        }
    }

    add_to_clear_diagonally_2(color_to_clear: string, j: number, i: number): void {
        let in_row: number = 0;
        for(let c = j; c < 9 - i / 9 && this.fields[i + c * 10] == color_to_clear; c++) {
            this.to_clear.push(i + c * 10);
            in_row++;
        }
        
        for(let c = j - 1; c >= 0 && this.fields[i + c * 10] == color_to_clear; c--) {
            this.to_clear.push(i + c * 10);
            in_row++;
        }
        
        this.score = this.score + 10 * Math.pow(2, in_row - 5);
    }

    check_diagonally_3(): void {
        for(let i = 8; i > 3; i--) {
            let current_color: string = 'none';
            let last_color: string = 'none'
            let number_in_color: number = 1;
            for(let j = 0; j < i + 1; j++) {
                current_color = this.fields[i + j * 8];
                if(current_color != 'none' && current_color == last_color) {
                    number_in_color++;
                } else {
                    number_in_color = 1;
                }
                
                if(number_in_color >= 5) {
                    this.add_to_clear_diagonally_3(current_color, j, i);
                    j = 9;
                }

                last_color = current_color;
            }
        }
    }

    add_to_clear_diagonally_3(color_to_clear: string, j: number, i: number): void {
        let in_row: number = 0;
        for(let c = j; c < i + 1 && this.fields[i + c * 8] == color_to_clear; c++) {
            this.to_clear.push(i + c * 8);
            in_row++;
        }
        
        for(let c = j - 1; c >= 0 && this.fields[i + c * 8] == color_to_clear; c--) {
            this.to_clear.push(i + c * 8);
            in_row++;
        }
        
        this.score = this.score + 10 * Math.pow(2, in_row - 5);
    }

    check_diagonally_4(): void {
        for(let i = 17; i <= 44; i = i + 9) {
            let current_color: string = 'none';
            let last_color: string = 'none'
            let number_in_color: number = 1;
            for(let j = 0; j < 9 - Math.floor(i / 9); j++) {
                current_color = this.fields[i + j * 8];
                if(current_color != 'none' && current_color == last_color) {
                    number_in_color++;
                } else {
                    number_in_color = 1;
                }
                
                if(number_in_color >= 5) {
                    this.add_to_clear_diagonally_4(current_color, j, i);
                    j = 9;
                }

                last_color = current_color;
            }
        }
    }

    add_to_clear_diagonally_4(color_to_clear: string, j: number, i: number): void {
        let in_row: number = 0;
        for(let c = j; c < 9 - Math.floor(i / 9) && this.fields[i + c * 8] == color_to_clear; c++) {
            this.to_clear.push(i + c * 8);
            in_row++;
        }
        
        for(let c = j - 1; c >= 0 && this.fields[i + c * 8] == color_to_clear; c--) {
            this.to_clear.push(i + c * 8);
            in_row++;
        }
        
        this.score = this.score + 10 * Math.pow(2, in_row - 5);
    }

    clear_rows(): void {
        this.to_clear.forEach(element => {
            this.fields[element] = 'none';
        });
        
        this.to_clear = new Array();
    }

    add_random(): void {
        let empty_fields: number[] = new Array();
        for(let i = 0; i < this.fields.length; i++) {
            if(this.fields[i] == 'none') {
                empty_fields.push(i);
            }
        }

        let random_colors: string[] = new Array();
        for(let i = 0; i < 3; i++) {
            random_colors.push(this.possible_colors[this.random_int_in_range(0, this.possible_colors.length - 1)]);
        }

        if(empty_fields.length > 3) {
            for(let i = 0; i < 3; i++) {
                let random_field_number: number = empty_fields[this.random_int_in_range(0, empty_fields.length - 1)];
                this.fields[random_field_number] = random_colors[i];
                empty_fields.splice(random_field_number, 1);
            }
        } else {
            for(let i = 0; i < empty_fields.length; ++i) {
                this.fields[empty_fields[i]] = random_colors[i];
            }

            this.game_over = true;
        }
    }

    new_game(): void {
        for(let i = 0; i < 81; i++) {
            this.fields[i] = 'none';
        }

        this.game_over = false;
        this.add_random();
    }

    random_int_in_range(min: number, max: number): number {
        return Math.floor(Math.random() * (max - min + 1) + min);
    }

}
