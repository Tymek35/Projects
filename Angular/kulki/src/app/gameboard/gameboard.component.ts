import { Component, OnInit } from '@angular/core';
import { GameService } from '../game.service';
import { ApiService } from '../api.service';
import { FormControl, Validators } from '@angular/forms';

@Component({
  selector: 'app-gameboard',
  templateUrl: './gameboard.component.html',
  styleUrls: ['./gameboard.component.scss']
})
export class GameboardComponent implements OnInit {
    fields: string[] = new Array(this.gameService.gameboard_size);
    choosen_field: number = -1;
    game_over: boolean = false;
    player_name_control = new FormControl('', [Validators.required, Validators.minLength(1), Validators.maxLength(14)]);
    saved: boolean = false;
    save_error: boolean = false;
    player_name_invalid: boolean = false;
    api_error: boolean = false;

    constructor(private gameService: GameService, private apiService: ApiService) {
        this.gameService.fields_subject.subscribe((arr) => {
            this.fields = arr;
        });
        this.gameService.game_over_subject.subscribe((over) => {
            this.game_over = over;
        });
    }

    ngOnInit(): void {
        if (this.gameService.first_game) {
            this.gameService.new_game();
        } else {
            this.gameService.continue_game();
        }
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
        this.player_name_control.enable();
        let save_button: HTMLButtonElement = <HTMLButtonElement>document.getElementById('save_button');
        save_button.disabled = false;
        this.saved = false;
    }

    save_score(): void {
        if(this.player_name_control.valid) {
            this.player_name_invalid = false;
            this.apiService.save_score(this.gameService.score, <string>this.player_name_control.value).subscribe({
                    next: (res) => {
                        this.player_name_control.disable();
                        let save_button: HTMLButtonElement = <HTMLButtonElement>document.getElementById('save_button');
                        save_button.disabled = true;
                        this.saved = true;
                        this.save_error = false;
                    },
                    error: (err) => {
                        console.log(err);
                        this.save_error = true;
                    }
            });
            
        } else {
            this.player_name_invalid = true;
        }
    }
}
