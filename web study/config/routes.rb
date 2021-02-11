Rails.application.routes.draw do
  resources :addresses_users
  resources :addresses
  resources :emails
  resources :professions
  resources :users
  get 'welcome/index'
  root 'welcome#index'
  get    'login'   => 'session#new'
  post   'login'   => 'session#create'
  delete 'logout'  => 'session#destroy'
  get    'logout'  => 'session#destroy'
end
