import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { GameWrapperComponent } from './game-wrapper/game-wrapper.component';
import { PageNotFoundComponent } from './page-not-found/page-not-found.component';
import { CurrienciesComponent } from './curriencies/curriencies.component';
import { GameRulesComponent } from './game-rules/game-rules.component';
import { ReadmeComponent } from './readme/readme.component';
import { KulkiAppWrapperComponent } from './kulki-app-wrapper/kulki-app-wrapper.component';

const routes: Routes = [
    {path: '', redirectTo: 'kulki', pathMatch: 'full'},
    {path: '', component: KulkiAppWrapperComponent, children: [
        {path: 'kulki', component: GameWrapperComponent},
        {path: 'zasady_gry', component: GameRulesComponent},
        {path: 'waluty', component: CurrienciesComponent},
        {path: 'readme', component: ReadmeComponent},
    ]},
    {path: '**', component: PageNotFoundComponent}
];

@NgModule({
    imports: [RouterModule.forRoot(routes)],
    exports: [RouterModule]
})
export class AppRoutingModule { }
