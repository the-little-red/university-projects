class ApplicationMailer < ActionMailer::Migration[5.0]
  default from: 'from@example.com'
  layout 'mailer'
end
