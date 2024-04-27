import { Component, OnInit, Input } from '@angular/core';
import { ScoreInterface } from '../api.service';

@Component({
  selector: 'app-score-card',
  templateUrl: './score-card.component.html',
  styleUrls: ['./score-card.component.scss']
})
export class ScoreCardComponent implements OnInit {
  @Input() score: ScoreInterface = {score: 0, playerName: "", scoreDate: new Date()};

  constructor() { }

  ngOnInit(): void {
  }

}
