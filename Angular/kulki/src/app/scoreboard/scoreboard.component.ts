import { Component, OnInit, Inject } from '@angular/core';
import { DOCUMENT } from '@angular/common';

@Component({
    selector: 'app-scoreboard',
    templateUrl: './scoreboard.component.html',
    styleUrls: ['./scoreboard.component.scss']
})
export class ScoreboardComponent implements OnInit {
    elem: any;
    fullscreen_on: boolean = false;
    next: string[] = ['blue', 'green', 'yellow']

    constructor(@Inject(DOCUMENT) private document: any) { }

    ngOnInit(): void {
        this.elem = document.documentElement;
    }

    fullscreen(): void {
        if(!this.fullscreen_on) {
            if (this.elem.requestFullscreen) {
                this.elem.requestFullscreen();
            } else if (this.elem.mozRequestFullScreen) {
                /* Firefox */
                this.elem.mozRequestFullScreen();
            } else if (this.elem.webkitRequestFullscreen) {
                /* Chrome, Safari and Opera */
                this.elem.webkitRequestFullscreen();
            } else if (this.elem.msRequestFullscreen) {
                /* IE/Edge */
                this.elem.msRequestFullscreen();
            }
            this.fullscreen_on = true;
        } else {
            if (this.document.exitFullscreen) {
                this.document.exitFullscreen();
            } else if (this.document.mozCancelFullScreen) {
                /* Firefox */
                this.document.mozCancelFullScreen();
            } else if (this.document.webkitExitFullscreen) {
                /* Chrome, Safari and Opera */
                this.document.webkitExitFullscreen();
            } else if (this.document.msExitFullscreen) {
                /* IE/Edge */
                this.document.msExitFullscreen();
            }
            this.fullscreen_on = false;
        }
    }

}
