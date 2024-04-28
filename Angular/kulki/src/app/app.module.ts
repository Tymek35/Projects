import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { ScoreboardComponent } from './scoreboard/scoreboard.component';
import { GameboardComponent } from './gameboard/gameboard.component';
import { GameWrapperComponent } from './game-wrapper/game-wrapper.component';
import { GameRulesComponent } from './game-rules/game-rules.component';
import { ScoresComponent } from './scores/scores.component';
import { FooterComponent } from './footer/footer.component';
import { NavigationBarComponent } from './navigation-bar/navigation-bar.component';
import { PageNotFoundComponent } from './page-not-found/page-not-found.component';
import { HttpClientModule } from '@angular/common/http';
import { ReadmeComponent } from './readme/readme.component';
import { KulkiAppWrapperComponent } from './kulki-app-wrapper/kulki-app-wrapper.component';
import { ScoreCardComponent } from './score-card/score-card.component';
import { ReactiveFormsModule } from '@angular/forms';
import { PortfolioComponent } from './portfolio/portfolio.component';

@NgModule({
  declarations: [
    AppComponent,
    ScoreboardComponent,
    GameboardComponent,
    GameWrapperComponent,
    GameRulesComponent,
    ScoresComponent,
    FooterComponent,
    NavigationBarComponent,
    PageNotFoundComponent,
    ReadmeComponent,
    KulkiAppWrapperComponent,
    ScoreCardComponent,
    PortfolioComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    ReactiveFormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
