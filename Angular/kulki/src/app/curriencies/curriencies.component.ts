import { HttpClient } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';
import { Observable, throwError } from 'rxjs';
import { catchError, retry } from 'rxjs/operators';

@Component({
    selector: 'app-curriencies',
    templateUrl: './curriencies.component.html',
    styleUrls: ['./curriencies.component.scss']
})
export class CurrienciesComponent implements OnInit {
    current_date: Date = new Date();
    currencies: string[] = ['eur', 'usd', 'gbp', 'jpy', 'chf', 'cny'];
    currencies_value: number[] = [0, 0, 0, 0, 0, 0];
    error: boolean = false;
    api_link: string = 'https://cdn.jsdelivr.net/gh/fawazahmed0/currency-api@1/latest/currencies/';
    
    constructor(private http: HttpClient) { }

    ngOnInit(): void {
        this.get_currencies();
    }

    get_currencies(): void {
        for(let i = 0; i < this.currencies.length; i++) {
            this.http.get<any>(this.api_link + this.currencies[i] + '/pln.json').subscribe(
                res => {
                    this.currencies_value[i] = Math.round(res.pln * 100) / 100;
                    this.error = false;
                },
              err => {
                    console.log(err);
                    this.error = true;
            });
        }
    }

}
