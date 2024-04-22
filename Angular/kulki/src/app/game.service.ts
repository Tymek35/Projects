import { Injectable } from '@angular/core';
import { BehaviorSubject, Subject } from 'rxjs';

@Injectable({
    providedIn: 'root'
})
export class GameService {
    score: number = 0;
    best: number = 0;
    score_subject: BehaviorSubject<number> = new BehaviorSubject(0);
    best_subject: BehaviorSubject<number> = new BehaviorSubject(0);
    readonly possible_colors: string[] = ['blue', 'purple', 'green', 'yellow', 'orange', 'brown', 'light_blue'];
    next: string[] = ['blue', 'blue', 'blue'];

    readonly gameboard_size: number = 81;
    readonly empty_field_constant: string = 'none';
    fields: string[] = new Array(this.gameboard_size);
    game_over_subject: BehaviorSubject<boolean> = new BehaviorSubject(false);
    to_clear: number[] = new Array();
    fields_subject: Subject<string[]> = new Subject();
    next_subject: Subject<string[]> = new Subject();
    first_game: boolean = true;

    constructor() {
        let best: any = localStorage.getItem('best');
        if(best != null) {
            this.best = parseInt(best);
            this.best_subject.next(this.best);
        }
    }

    add_to_score(value: number): void {
        this.score = this.score + value;
        this.score_subject.next(this.score);
        if(this.score > this.best) {
            this.best = this.score;
            this.best_subject.next(this.best);
            localStorage.setItem('best', this.best.toString())
        }
    }

    set_score(value: number): void {
        this.score = value;
    }

    random_next(): void {
        let random_colors: string[] = new Array();
        for(let i = 0; i < 3; i++) {
            random_colors.push(this.possible_colors[this.random_int_in_range(0, this.possible_colors.length - 1)]);
        }
        this.next = random_colors;
        this.next_subject.next(this.next);
    }

    random_int_in_range(min: number, max: number): number {
        return Math.floor(Math.random() * (max - min + 1) + min);
    }

    choose_field(field_num: number, choosen_field: number): number {
        if(choosen_field != -1 && this.check_if_move_possible(field_num, choosen_field)) {
            this.fields[field_num] = this.fields[choosen_field];
            this.fields[choosen_field] = this.empty_field_constant;
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
            return -1;
        }

        return choosen_field;
    }

    initialize_gameboard(): void {
        for(let i = 0; i < this.gameboard_size; i++) {
            this.fields[i] = this.empty_field_constant;
        }
    }

    check_if_move_possible(field_num: number, choosen_field: number): boolean {
        let stack: number[] = new Array();
        stack.push(choosen_field);
        let fields_visited: boolean[] = new Array(this.gameboard_size);
        for(let i = 0; i < this.gameboard_size; i++) {
            fields_visited[i] = false;
        }
        let reached: boolean = false;

        while(!reached && stack.length != 0) {
            let field: any = stack.pop();
            if(field == field_num) {
                reached = true;
            } else {
                fields_visited[field] = true;
                if(field - 9 >= 0 && this.fields[field - 9] == this.empty_field_constant && !fields_visited[field - 9]) {
                    stack.push(field - 9);
                }
                if(field - 1 >= 0 && field % 9 != 0 && this.fields[field - 1] == this.empty_field_constant && !fields_visited[field - 1]) {
                    stack.push(field - 1);
                }
                if(field + 1 < this.gameboard_size && field % 9 != 8 && this.fields[field + 1] == this.empty_field_constant && !fields_visited[field + 1]) {
                    stack.push(field + 1);
                }
                if(field + 9 < this.gameboard_size && this.fields[field + 9] == this.empty_field_constant && !fields_visited[field + 9]) {
                    stack.push(field + 9);
                }
            }
        }

        return reached;
    }

    check_horizontally(): void {
        for(let i = 0; i < 9; i++) {
            let current_color: string = this.empty_field_constant;
            let last_color: string = this.empty_field_constant
            let number_in_color: number = 1;
            for(let j = 0; j < 9; j++) {
                current_color = this.fields[i * 9 + j];
                if(current_color != this.empty_field_constant && current_color == last_color) {
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
        
        this.add_to_score(10 * Math.pow(2, in_row - 5));
    }

    check_vertically(): void {
        for(let i = 0; i < 9; i++) {
            let current_color: string = this.empty_field_constant;
            let last_color: string = this.empty_field_constant;
            let number_in_color: number = 1;
            for(let j = 0; j < 9; j++) {
                current_color = this.fields[i + j * 9];
                if(current_color != this.empty_field_constant && current_color == last_color) {
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
        
        this.add_to_score(10 * Math.pow(2, in_row - 5));
    }

    // Checks if at least five balls of the same color are in line diagonally
    check_diagonally(): void {
        this.check_diagonally_1();
        this.check_diagonally_2();
        this.check_diagonally_3();
        this.check_diagonally_4();
    }

    // Checks diagonally from up left to down right on diagonal and above
    check_diagonally_1(): void {
        for(let i = 0; i < 5; i++) {
            let current_color: string = this.empty_field_constant;
            let last_color: string = this.empty_field_constant;
            let number_in_color: number = 1;
            for(let j = 0; j < 9 - i; j++) {
                current_color = this.fields[i + j * 10];
                if(current_color != this.empty_field_constant && current_color == last_color) {
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
        
        this.add_to_score(10 * Math.pow(2, in_row - 5));
    }

    // Checks diagonally from up left to down right below the diagonal
    check_diagonally_2(): void {
        for(let i = 9; i <= 36; i = i + 9) {
            let current_color: string = this.empty_field_constant;
            let last_color: string = this.empty_field_constant;
            let number_in_color: number = 1;
            for(let j = 0; j < 9 - i / 9; j++) {
                current_color = this.fields[i + j * 10];
                if(current_color != this.empty_field_constant && current_color == last_color) {
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
        
        this.add_to_score(10 * Math.pow(2, in_row - 5));
    }

    // Checks diagonally from up right to down left on diagonal and above
    check_diagonally_3(): void {
        for(let i = 8; i > 3; i--) {
            let current_color: string = this.empty_field_constant;
            let last_color: string = this.empty_field_constant;
            let number_in_color: number = 1;
            for(let j = 0; j < i + 1; j++) {
                current_color = this.fields[i + j * 8];
                if(current_color != this.empty_field_constant && current_color == last_color) {
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
        
        this.add_to_score(10 * Math.pow(2, in_row - 5));
    }

    // Checks diagonally from up right to down left below the diagonal
    check_diagonally_4(): void {
        for(let i = 17; i <= 44; i = i + 9) {
            let current_color: string = this.empty_field_constant;
            let last_color: string = this.empty_field_constant;
            let number_in_color: number = 1;
            for(let j = 0; j < 9 - Math.floor(i / 9); j++) {
                current_color = this.fields[i + j * 8];
                if(current_color != this.empty_field_constant && current_color == last_color) {
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
        
        this.add_to_score(10 * Math.pow(2, in_row - 5));
    }

    clear_rows(): void {
        this.to_clear.forEach(element => {
            this.fields[element] = this.empty_field_constant;
        });
        
        this.to_clear = new Array();
        this.fields_subject.next(this.fields);
    }

    add_random(): void {
        let empty_fields: number[] = new Array();
        for(let i = 0; i < this.fields.length; i++) {
            if(this.fields[i] == this.empty_field_constant) {
                empty_fields.push(i);
            }
        }

        let random_colors: string[] = this.next;
        this.random_next();

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

            this.game_over_subject.next(true);
        }

        this.fields_subject.next(this.fields);
    }

    new_game(): void {
        this.initialize_gameboard();
        this.random_next();
        this.game_over_subject.next(false);
        this.score = 0;
        this.score_subject.next(0);
        this.add_random();
        this.fields_subject.next(this.fields);
        this.first_game = false;
    }

    continue_game(): void {
        this.score_subject.next(0);
        this.fields_subject.next(this.fields);
        this.next_subject.next(this.next);
        this.best_subject.next(this.best);
    }
}
