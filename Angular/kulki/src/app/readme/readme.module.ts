import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { ReadmeRoutingModule } from './readme-routing.module';
import { ReadmeComponent } from './readme.component';


@NgModule({
  declarations: [
    ReadmeComponent
  ],
  imports: [
    CommonModule,
    ReadmeRoutingModule
  ]
})
export class ReadmeModule { }
