import { ComponentFixture, TestBed } from '@angular/core/testing';

import { KulkiAppWrapperComponent } from './kulki-app-wrapper.component';

describe('KulkiAppWrapperComponent', () => {
  let component: KulkiAppWrapperComponent;
  let fixture: ComponentFixture<KulkiAppWrapperComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ KulkiAppWrapperComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(KulkiAppWrapperComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
