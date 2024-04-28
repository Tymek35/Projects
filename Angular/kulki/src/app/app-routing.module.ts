import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { GameWrapperComponent } from './game-wrapper/game-wrapper.component';
import { PageNotFoundComponent } from './page-not-found/page-not-found.component';
import { ScoresComponent } from './scores/scores.component';
import { GameRulesComponent } from './game-rules/game-rules.component';
import { ReadmeComponent } from './readme/readme.component';
import { KulkiAppWrapperComponent } from './kulki-app-wrapper/kulki-app-wrapper.component';
import { PortfolioComponent } from './portfolio/portfolio.component';

const routes: Routes = [
    {path: '', redirectTo: 'kulki', pathMatch: 'full'},
    {path: 'portfolio', component: PortfolioComponent},
    {path: '', component: KulkiAppWrapperComponent, children: [
        {path: 'kulki', component: GameWrapperComponent},
        {path: 'zasady_gry', component: GameRulesComponent},
        {path: 'wyniki', component: ScoresComponent},
        {path: 'readme', component: ReadmeComponent},
    ]},
    {path: 'lazy_loading', loadChildren: () => import('./lazy-loading/lazy-loading.module').then(m => m.LazyLoadingModule)},
    {path: '**', component: PageNotFoundComponent}
];

@NgModule({
    imports: [RouterModule.forRoot(routes)],
    exports: [RouterModule]
})
export class AppRoutingModule { }
