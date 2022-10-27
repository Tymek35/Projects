import { Injectable } from '@angular/core';
import { BehaviorSubject } from 'rxjs';

@Injectable({
    providedIn: 'root'
})
export class GameService {
    score: number = 0;
    best: number = 0;
    private new_game = new BehaviorSubject('');
    new_game_observable = this.new_game.asObservable();
    possible_colors: string[] = ['blue', 'purple', 'green', 'yellow', 'orange', 'brown', 'light_blue'];
    next: string[] = ['blue', 'blue', 'blue'];

    constructor() {
        let best: any = localStorage.getItem('best');
        if(best != null) {
            this.best = parseInt(best);
        }
        this.random_next();
    }

    get_score(): number {
        return this.score;
    }

    add_to_score(value: number): void {
        this.score = this.score + value;
        if(this.score > this.best) {
            this.best = this.score;
            localStorage.setItem('best', this.best.toString())
        }
    }

    set_score(value: number): void {
        this.score = value;
    }

    get_best(): number {
        return this.best;
    }

    new_game_call(): void {
        this.new_game.next('');
    }

    get_next(): string[] {
        return this.next;
    }

    random_next(): void {
        let random_colors: string[] = new Array();
        for(let i = 0; i < 3; i++) {
            random_colors.push(this.possible_colors[this.random_int_in_range(0, this.possible_colors.length - 1)]);
        }
        this.next = random_colors;
    }

    random_int_in_range(min: number, max: number): number {
        return Math.floor(Math.random() * (max - min + 1) + min);
    }
}
