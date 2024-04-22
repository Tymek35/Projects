import { Component, OnInit } from '@angular/core';
import { GameService } from '../game.service';

@Component({
  selector: 'app-gameboard',
  templateUrl: './gameboard.component.html',
  styleUrls: ['./gameboard.component.scss']
})
export class GameboardComponent implements OnInit {
    fields: string[] = new Array(this.gameService.gameboard_size);
    choosen_field: number = -1;
    game_over: boolean = false;

    constructor(private gameService: GameService) {
        this.gameService.fields_subject.subscribe((arr) => {
            this.fields = arr;
        });
        this.gameService.game_over_subject.subscribe((over) => {
            this.game_over = over;
        });
    }

    ngOnInit(): void {
        this.gameService.new_game();
    }

    choose_field(field_num: number): void {
        if(this.fields[field_num] != this.gameService.empty_field_constant) {
            this.choosen_field = field_num;
        } else {
            this.choosen_field = this.gameService.choose_field(field_num, this.choosen_field);
        }
    }

    new_game(): void {
        this.gameService.new_game();
    }
}
