import { Injectable } from '@angular/core';
import { HttpClient, HttpResponse } from '@angular/common/http';
import { Observable } from 'rxjs';

export interface ScoreInterface {
    scoreDate: Date;
    playerName: string;
    score: number;
}

@Injectable({
    providedIn: 'root'
})
export class ApiService {
    private readonly apiURL: string = "http://localhost:8000";
    public scores: ScoreInterface[] = [];
    public error: boolean = false;

    constructor(private http: HttpClient) { }

    get_scores(): void {
        this.http.get<ScoreInterface[]>(this.apiURL + '/scores/getScore/').subscribe({
            next: (res) => {
                this.scores = res;
                this.error = false;
            },
            error: (err) => {
                console.log(err);
                this.error = true;
            }
        });
    }

    save_score(score: number, player_name: string): Observable<HttpResponse<any>> {
        let body = {'playerName': player_name, 'score': score};
        return this.http.put<HttpResponse<any>>(this.apiURL + '/scores/saveScore/', body);
    }
}
