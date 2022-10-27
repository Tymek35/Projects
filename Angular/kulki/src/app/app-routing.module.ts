import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { GameWrapperComponent } from './game-wrapper/game-wrapper.component';
import { PageNotFoundComponent } from './page-not-found/page-not-found.component';
import { CurrienciesComponent } from './curriencies/curriencies.component';
import { GameRulesComponent } from './game-rules/game-rules.component';

const routes: Routes = [
    {path: '', redirectTo: '/kulki', pathMatch: 'full'},
    {path: 'kulki', component: GameWrapperComponent},
    {path: 'zasady_gry', component: GameRulesComponent},
    {path: 'waluty', component: CurrienciesComponent},
    {path: 'readme', loadChildren: () => import('./readme/readme.module').then(m => m.ReadmeModule)},
    {path: '**', component: PageNotFoundComponent}
];

@NgModule({
    imports: [RouterModule.forRoot(routes)],
    exports: [RouterModule]
})
export class AppRoutingModule { }
