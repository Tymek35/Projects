import { Component, OnInit } from '@angular/core';
import { ApiService } from '../api.service';

@Component({
    selector: 'app-scores',
    templateUrl: './scores.component.html',
    styleUrls: ['./scores.component.scss']
})
export class ScoresComponent implements OnInit {
    current_date: Date = new Date();
    error: boolean = false;
    
    constructor(public apiService: ApiService) { }

    ngOnInit(): void {
        this.get_scores();
    }

    get_scores(): void {
        this.apiService.get_scores();
    }

}
